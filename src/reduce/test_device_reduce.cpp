/* Add the following IR code to /opt/rocm/hip/lib/hip_hc_gfx803.ll

define float @__hip_hc_cu_reducex64_f32(float %a) #1 {
  %1 = tail call float asm sideeffect "
            v_add_f32 $0, $1, $1 row_shr:1 bound_ctrl:0
            v_add_f32 $0, $1, $0 row_shr:2 bound_ctrl:0
            v_add_f32 $0, $1, $0 row_shr:3 bound_ctrl:0
            v_nop
            v_nop
            v_add_f32 $0, $0, $0 row_shr:4 bank_mask:0xe
            v_nop
            v_nop
            v_add_f32 $0, $0, $0 row_shr:8 bank_mask:0xc
            v_nop
            v_nop
            v_add_f32 $0, $0, $0 row_bcast:15 row_mask:0xa
            v_nop
            v_nop
            v_add_f32 $0, $0, $0 row_bcast:31 row_mask:0xc
            ","=v,v"(float %a)
  ret float %1
}
*/

#include<hip/hip_runtime.h>
#include<hip/hip_runtime_api.h>
#include<iostream>
#include<cassert>

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
        if((NUM_THREADS >= 512) && (tid < 256)) {
            sum = sum + sdata[tid + 256];
            sdata[tid] = sum;
        }
        __syncthreads();
        if((NUM_THREADS >= 256) && (tid < 128)) {
            sum = sum + sdata[tid + 128];
            sdata[tid] = sum;
        }
        __syncthreads();
        if((NUM_THREADS >= 128) && (tid < 64)) {
            sum = sum + sdata[tid + 64];
            sdata[tid] = sum;
        }
        __syncthreads();
        if((NUM_THREADS >= 64) && (tid < 32)) {
            sum = sum + sdata[tid + 32];
            sdata[tid] = sum;
        }
        __syncthreads();
        if((NUM_THREADS >= 32) && (tid < 16)) {
            sum = sum + sdata[tid + 16];
            sdata[tid] = sum;
        }
        __syncthreads();
        if((NUM_THREADS >= 16) && (tid < 8)) {
            sum = sum + sdata[tid + 8];
            sdata[tid] = sum;
        }
        __syncthreads();
        if((NUM_THREADS >= 8) && (tid < 4)) {
            sum = sum + sdata[tid + 4];
            sdata[tid] = sum;
        }
        __syncthreads();
        if((NUM_THREADS >= 4) && (tid < 2)) {
            sum = sum + sdata[tid + 2];
            sdata[tid] = sum;
        }
        __syncthreads();
        if((NUM_THREADS >= 2) && (tid < 1)) {
            sum = sum + sdata[tid + 1];
            sdata[tid] = sum;
        }
        __syncthreads();
        return sum;
    }
	__device__ ~ReductionBlock(){}
};


extern "C" float __hip_hc_cu_reducex64_f32(float);

template<typename T, int NUM_THREADS>
class ReductionBlockDPP{
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

        if((NUM_THREADS >= 128) && (tid < 64)) {
            sum = sum + sdata[tid + 64];
            sdata[tid] = sum;
        }
        __syncthreads();

        sum = __hip_hc_cu_reducex64_f32(sum);

        return sum;
    }
	__device__ ~ReductionBlockDPP(){}
};



template<typename T, int NUM_THREADS>
__global__ void BlockSumKernel(
    T *d_in,
    T *d_out,
    int n)
{
    ReductionBlock<T, NUM_THREADS> R;
    R.Load(d_in, n);
    int val = R.Sum();
    if(hipThreadIdx_x == 0) d_out[hipBlockIdx_x] = val;
}

float Initialize(float *h_in, float *h_out, int num_items)
{
    float inclusive = 0;
    for(int i=0;i<num_items;++i)
    {
        h_in[i] = 10;
        h_out[i] = 0;
        inclusive += h_in[i];
    }
    return inclusive;
}

template<typename T>
void Launcher(T *d_in, T *d_out, int N, dim3 dimGrid){
    int n = 1;
    dim3 dimBlock(1,1,1);

    if(N > 64 && N <= 128) {
        dimBlock.x = 128;
    }
    if(N > 32 && N <= 64) {
        dimBlock.x = 64;
    }
    if(N > 16 && N <= 32) {
        dimBlock.x = 32;
    }
    if(N > 8  && N <= 16) {
        dimBlock.x = 16;
    }
    if(N > 4 && N <= 8) {
        dimBlock.x = 8;
    }
    if(N > 2 && N <= 4) {
        dimBlock.x = 4;
    }
    if(N > 1 && N <= 2){
        dimBlock.x = 2;
    }
    if(N == 1) {
        dimBlock.x = 1;
    }

    switch(dimBlock.x){
        case 128:
            hipLaunchKernelGGL((BlockSumKernel<T, 128>), dimGrid, dimBlock, 0, 0, d_in, d_out, N); break;
        case 64:
            hipLaunchKernelGGL((BlockSumKernel<T, 64>), dimGrid, dimBlock, 0, 0, d_in, d_out, N); break;
        case 32:
            hipLaunchKernelGGL((BlockSumKernel<T, 32>), dimGrid, dimBlock, 0, 0, d_in, d_out, N); break;
        case 16:
            hipLaunchKernelGGL((BlockSumKernel<T, 16>), dimGrid, dimBlock, 0, 0, d_in, d_out, N); break;
        case 8:
            hipLaunchKernelGGL((BlockSumKernel<T, 8>),  dimGrid, dimBlock, 0, 0, d_in, d_out, N); break;
        case 4:
            hipLaunchKernelGGL((BlockSumKernel<T, 4>),  dimGrid, dimBlock, 0, 0, d_in, d_out, N); break;
        case 2:
            hipLaunchKernelGGL((BlockSumKernel<T, 2>),  dimGrid, dimBlock, 0, 0, d_in, d_out, N); break;
        case 1:
            hipLaunchKernelGGL((BlockSumKernel<T, 1>),  dimGrid, dimBlock, 0, 0, d_in, d_out, N); break;
    }
   
}

template<typename T>
void DeviceReduce(T *d_in, T *d_out, int N)
{
    if(N <= 128) {
        Launcher<T>(d_in, d_out, N, dim3(1,1,1));
    }
    if(N > 128) {
        dim3 launch0(N/128 + N % 128 > 1 ? 1 : 0, 1, 1);
        Launcher<T>(d_in, d_out, N, launch0);
        Launcher<T>(d_out, d_out, launch0.x, dim3(1,1,1));
    }
}

int main()
{
    for(int BLOCK_THREADS= 2048; BLOCK_THREADS > 1; BLOCK_THREADS = BLOCK_THREADS - 5){
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
    DeviceReduce<float>(d_in, d_out, BLOCK_THREADS);
    hipMemcpy(h_gpu, d_out, sizeof(float), hipMemcpyDeviceToHost);
    if(h_gpu[0] != h_aggregate) {
        std::cout<<"Error at: "<<BLOCK_THREADS<<std::endl;
        std::cout<<"Got: "<<h_gpu[0]<<" Expected: "<<h_aggregate<<std::endl;
        exit(0);
    }
    std::cout<<"Running at N = "<<BLOCK_THREADS<<std::endl; 
    hipFree(d_in);
    hipFree(d_out);
    delete[] h_in;
    delete[] h_gpu;
    } 
}

