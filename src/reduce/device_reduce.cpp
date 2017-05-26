#include<hip/hip_runtime_api.h>
#include<hip/hip_runtime.h>
#include<iostream>

#define LEN 512*512
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

template<typename T>
void Launcher(T *in, T *out, dim3 dimGrid, dim3 dimBlock, unsigned int len) {
    switch(dimBlock.x) {
        case 512:
            hipLaunchKernelGGL((ReduceLane64<float, 512, true>), dimGrid, dimBlock, 0, 0, in, out, len); break;
        case 256:
            hipLaunchKernelGGL((ReduceLane64<float, 256, true>), dimGrid, dimBlock, 0, 0, in, out, len); break;
        case 128:
            hipLaunchKernelGGL((ReduceLane64<float, 128, true>), dimGrid, dimBlock, 0, 0, in, out, len); break;
        case  64:
            hipLaunchKernelGGL((ReduceLane64<float,  64, true>), dimGrid, dimBlock, 0, 0, in, out, len); break;
        case  32:
            hipLaunchKernelGGL((ReduceLane64<float,  32, true>), dimGrid, dimBlock, 0, 0, in, out, len); break;
        case  16:
            hipLaunchKernelGGL((ReduceLane64<float,  16, true>), dimGrid, dimBlock, 0, 0, in, out, len); break;
        case   8:
            hipLaunchKernelGGL((ReduceLane64<float,   8, true>), dimGrid, dimBlock, 0, 0, in, out, len); break;
        case   4:
            hipLaunchKernelGGL((ReduceLane64<float,   4, true>), dimGrid, dimBlock, 0, 0, in, out, len); break;
        case   2:
            hipLaunchKernelGGL((ReduceLane64<float,   2, true>), dimGrid, dimBlock, 0, 0, in, out, len); break;
        case   1:
            hipLaunchKernelGGL((ReduceLane64<float,   1, true>), dimGrid, dimBlock, 0, 0, in, out, len); break;

    }
}

template<typename T>
void DeviceReduce(T* in, T *out, unsigned int len) {
    dim3 dimGrid0(1,1,1);
    dim3 dimBlock0(1,1,1);
    dim3 dimGrid1(1,1,1);
    dim3 dimBlock1(1,1,1);

    if(len <= 1024) {
        dimBlock0.x = len;
        Launcher(in, out, dimGrid0, dimBlock0, len);
    }

    if((len <= 1024 * 1024) && (len > 1024)) {
        dimBlock0.x = 1024;
        dimGrid0.x = len/1024;
        dimBlock1.x = len/1024;
        dimGrid1.x = 1;
        Launcher(in, out, dimGrid0, dimBlock0, len);
        Launcher(in, out, dimGrid1, dimBlock1, len);
    }

    

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

//    hipLaunchKernelGGL((ReduceLane64<float, 512, true>), dim3(LEN/512,1,1), dim3(512,1,1), 0, 0, Ad, Bd, LEN);
    DeviceReduce(Ad, Bd, LEN);
	hipDeviceSynchronize();
    hipMemcpy(B, Bd, SIZE, hipMemcpyDeviceToHost);

    for(int i=0;i<65536/1024 + 1;i++) { std::cout<< B[i] << std::endl; }
    std::cout<<B[0]<<std::endl;
	delete A;
	delete B;
	hipFree(Ad);
	hipFree(Bd);
}
