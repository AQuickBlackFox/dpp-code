#include<iostream>
#include<hip/hip_runtime.h>
#include<hip/hip_runtime_api.h>

typedef float U;

#define LEN 64
#define SIZE (LEN * sizeof(U))

template<typename T, int shift>
__global__ void Kernel(T *Out, T *In1, T *In2) {
    int tx = hipThreadIdx_x;
    if(shift == 1) {
        asm volatile ("v_add_f32 %0, %1, %2 row_shr:1 bound_ctrl:0" : "=v"(Out[tx]) : "v"(In1[tx]), "v"(In2[tx]));
    }
    if(shift == 2) {
        asm volatile ("v_add_f32 %0, %1, %2 row_shr:2 bound_ctrl:0" : "=v"(Out[tx]) : "v"(In1[tx]), "v"(In2[tx]));
    }
    if(shift == 4) {
        asm volatile ("v_add_f32 %0, %1, %2 row_shr:4 bound_ctrl:0" : "=v"(Out[tx]) : "v"(In1[tx]), "v"(In2[tx]));
    }
    if(shift == 8) {
        asm volatile ("v_add_f32 %0, %1, %2 row_shr:8 bound_ctrl:0" : "=v"(Out[tx]) : "v"(In1[tx]), "v"(In2[tx]));
    }
    if(shift == 15) {
        asm volatile ("v_add_f32 %0, %1, %2 row_shr:15 bound_ctrl:0" : "=v"(Out[tx]) : "v"(In1[tx]), "v"(In2[tx]));
    }

}

int main() {
    U *hIn1, *hIn2, *hOut, *dIn1, *dIn2, *dOut;
    hIn1 = new U[LEN];
    hIn2 = new U[LEN];
    hOut = new U[LEN];
    for(int i=0;i<LEN;i++) {
        hIn1[i] = 1.0f;
        hIn2[i] = 2.0f;
        hOut[i] = 0.0f;
    }

    hipMalloc(&dIn1, SIZE);
    hipMalloc(&dIn2, SIZE);
    hipMalloc(&dOut, SIZE);

    hipMemcpy(dIn1, hIn1, SIZE, hipMemcpyHostToDevice);
    hipMemcpy(dIn2, hIn2, SIZE, hipMemcpyHostToDevice);
    hipMemcpy(dOut, hOut, SIZE, hipMemcpyHostToDevice);

    std::cout<<"===== Shift Width : 1 ====="<<std::endl;

    hipLaunchKernelGGL((Kernel<U, 1>), dim3(1,1,1), dim3(LEN,1,1), 0, 0, dOut, dIn1, dIn2);
    hipDeviceSynchronize();

    hipMemcpy(hOut, dOut, SIZE, hipMemcpyDeviceToHost);
    for(int j=0;j<8;j++){
        for(int i=0;i<8;i++) {
            std::cout<<hOut[i+j*8]<<" ";
        }
        std::cout<<std::endl;
    }

    std::cout<<"===== Shift Width : 2 ====="<<std::endl;

    hipLaunchKernelGGL((Kernel<U, 2>), dim3(1,1,1), dim3(LEN,1,1), 0, 0, dOut, dIn1, dIn2);
    hipDeviceSynchronize();

    hipMemcpy(hOut, dOut, SIZE, hipMemcpyDeviceToHost);
    for(int j=0;j<8;j++){
        for(int i=0;i<8;i++) {
            std::cout<<hOut[i+j*8]<<" ";
        }
        std::cout<<std::endl;
    }

    std::cout<<"===== Shift Width : 4 ====="<<std::endl;

    hipLaunchKernelGGL((Kernel<U, 4>), dim3(1,1,1), dim3(LEN,1,1), 0, 0, dOut, dIn1, dIn2);
    hipDeviceSynchronize();

    hipMemcpy(hOut, dOut, SIZE, hipMemcpyDeviceToHost);
    for(int j=0;j<8;j++){
        for(int i=0;i<8;i++) {
            std::cout<<hOut[i+j*8]<<" ";
        }
        std::cout<<std::endl;
    }

    std::cout<<"===== Shift Width : 8 ====="<<std::endl;

    hipLaunchKernelGGL((Kernel<U, 8>), dim3(1,1,1), dim3(LEN,1,1), 0, 0, dOut, dIn1, dIn2);
    hipDeviceSynchronize();

    hipMemcpy(hOut, dOut, SIZE, hipMemcpyDeviceToHost);
    for(int j=0;j<8;j++){
        for(int i=0;i<8;i++) {
            std::cout<<hOut[i+j*8]<<" ";
        }
        std::cout<<std::endl;
    }

    std::cout<<"===== Shift Width : 15 ====="<<std::endl;

    hipLaunchKernelGGL((Kernel<U, 15>), dim3(1,1,1), dim3(LEN,1,1), 0, 0, dOut, dIn1, dIn2);
    hipDeviceSynchronize();

    hipMemcpy(hOut, dOut, SIZE, hipMemcpyDeviceToHost);
    for(int j=0;j<8;j++){
        for(int i=0;i<8;i++) {
            std::cout<<hOut[i+j*8]<<" ";
        }
        std::cout<<std::endl;
    }

}
