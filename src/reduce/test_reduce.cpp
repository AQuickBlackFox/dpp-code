#include<hip/hip_runtime.h>
#include<hip/hip_runtime_api.h>
#include<iostream>

extern "C" float __hip_hc_cu_reducex64_f32(float);

template<typename T, int NUM_THREADS>
class ReductionBlock{
    T sum = 0;
public:
    __device__ void Load(T* data, int n) {
        unsigned i = hipBlockIdx_x * NUM_THREADS * 2 + hipThreadIdx_x;
        unsigned gridSize = NUM_THREADS * 2 * hipGridDim_x;
        while(i < n) {
            sum += data[i];
            if(i + NUM_THREADS < n) {
                sum += data[i+NUM_THREADS];
            }
            i += gridSize;
        }
    }
    __device__ T Sum(){

        __shared__ T sdata[NUM_THREADS];
        int tid = hipThreadIdx_x;
        sdata[tid] = sum;
        __syncthreads();

        if((NUM_THREADS >= 128) && (tid < 64)){
            sum = sum + sdata[tid + 64];
            sdata[tid] = sum;
        }
        __syncthreads();

        sum = __hip_hc_cu_reducex64_f32(sum);

        return sum;
    }
	__device__ ~ReductionBlock(){}
};

template<typename T, int NUM_THREADS>
__global__ void BlockSumKernel(
    T *d_in,
    T *d_out,
    int n)
{
    ReductionBlock<T, NUM_THREADS> R;
    R.Load(d_in, n);
    float val = R.Sum();
    if(hipThreadIdx_x == 63) d_out[hipBlockIdx_x] = val;
}

float Initialize(float *h_in, float *h_out, int num_items)
{
    float inclusive = 0.0f;
    for(int i=0;i<num_items;++i)
    {
        h_in[i] = 10.0f;
        h_out[i] = 0.0f;
        inclusive += h_in[i];
    }
	return inclusive;
}

int main()
{
    const int BLOCK_THREADS = 128;
    const int ITEMS_PER_THREAD = 1;

    const int TILE_SIZE = BLOCK_THREADS * ITEMS_PER_THREAD;
    float *h_in = new float[TILE_SIZE];
    float *h_gpu = new float[TILE_SIZE];
    float h_aggregate = Initialize(h_in, h_gpu, TILE_SIZE);

    float *d_in, *d_out;
    hipMalloc(&d_in, TILE_SIZE * sizeof(float));
    hipMalloc(&d_out, TILE_SIZE * sizeof(float));

    hipMemcpy(d_in, h_in, sizeof(float) * TILE_SIZE, hipMemcpyHostToDevice);
    hipMemcpy(d_out, h_gpu, sizeof(float) * TILE_SIZE, hipMemcpyHostToDevice);

    hipLaunchKernelGGL((BlockSumKernel<float, 128>), dim3(1,1,1), dim3(BLOCK_THREADS,1,1), 0, 0, d_in, d_out, BLOCK_THREADS);
    hipMemcpy(h_gpu, d_out, sizeof(float), hipMemcpyDeviceToHost);
    std::cout<<h_gpu[0]<<" "<<std::endl;
}

