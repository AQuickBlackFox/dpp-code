#include<hip/hip_runtime_api.h>
#include<hip/hip_runtime.h>
#include<iostream>

#define LEN 32
#define SIZE LEN << 2

extern __attribute__((const)) int __mov_b32(int, int, int, int, bool) __asm("llvm.amdgcn.mov.dpp.i32");

__global__ void ReduceLane16(int *A, int *B)
{
    int tx = hipThreadIdx_x;
    B[tx] = __mov_b32(A[tx], 0x111, 1, 1, 1);
}

int main(){
    int *A, *B, *Ad, *Bd;
    A = new int[LEN];
    B = new int[LEN];

    for(int i=0;i<LEN;i++) {
        A[i] = i;
        B[i] = 0;
    }

    hipMalloc(&Ad, SIZE);
    hipMalloc(&Bd, SIZE);

    hipMemcpy(Ad, A, SIZE, hipMemcpyHostToDevice);
    hipMemcpy(Bd, B, SIZE, hipMemcpyHostToDevice);

    hipLaunchKernelGGL(ReduceLane16, dim3(1,1,1), dim3(LEN,1,1), 0, 0, Ad, Bd);
	hipDeviceSynchronize();
    hipMemcpy(B, Bd, SIZE, hipMemcpyDeviceToHost);

    for(int i=0;i<LEN;i++) { std::cout<< B[i] << std::endl; }
	delete A;
	delete B;
	hipFree(Ad);
	hipFree(Bd);
}
