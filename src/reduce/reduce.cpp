#include<hip/hip_runtime_api.h>
#include<hip/hip_runtime.h>
#include<iostream>

#define LEN 1024
#define SIZE LEN << 2

extern "C" void __hip_hc_cu_reducex64_f32(float, float);

template<typename T, unsigned int blockSize, bool nIsPow2>
__global__ void ReduceLane64(T *i_data, T *o_data, unsigned int n)
{
    unsigned int tid = hipThreadIdx_x;
    unsigned int i  = hipBlockIdx_x * blockSize * 2 + hipThreadIdx_x;
    unsigned int gridSize = blockSize * 2 * hipGridDim_x;

    T sum = 0;
    T val = o_data[tid];

    __shared__ T sdata[blockSize];

    while(i < n) {
        sum += i_data[i];
        if(nIsPow2 || i + blockSize < n) {
            sum += i_data[i+blockSize];
        }
        i += gridSize;
    }

    sdata[tid] = sum;
    __syncthreads();

    if((blockSize >= 512) && (tid < 256)){
        sdata[tid] = sum = sum + sdata[tid + 256];
    }
    __syncthreads();


    if((blockSize >= 256) && (tid < 128)){
        sdata[tid] = sum = sum + sdata[tid + 128];
    }
    __syncthreads();


    if((blockSize >= 128) && (tid < 64)){
        sdata[tid] = sum = sum + sdata[tid + 64];
    }
    __syncthreads();

    __hip_hc_cu_reducex64_f32(val, sdata[tid]);
    if(tid == 63) { o_data[hipBlockIdx_x] = val; }
}

int main(){
    float *A, *B, *Ad, *Bd;
    A = new float[LEN];
    B = new float[LEN];

    for(int i=0;i<LEN;i++) {
        A[i] = 1.0f;
        B[i] = 0;
    }

    hipMalloc(&Ad, SIZE);
    hipMalloc(&Bd, SIZE);

    hipMemcpy(Ad, A, SIZE, hipMemcpyHostToDevice);
    hipMemcpy(Bd, B, SIZE, hipMemcpyHostToDevice);

    hipLaunchKernelGGL((ReduceLane64<float, 512, true>), dim3(LEN/512,1,1), dim3(512,1,1), 0, 0, Ad, Bd, LEN);
	hipDeviceSynchronize();
    hipMemcpy(B, Bd, SIZE, hipMemcpyDeviceToHost);

    for(int i=0;i<4;i++) { std::cout<< B[i] << std::endl; }
    std::cout<<B[0]<<std::endl;
	delete A;
	delete B;
	hipFree(Ad);
	hipFree(Bd);
}
