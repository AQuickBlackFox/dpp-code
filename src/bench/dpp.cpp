#include<iostream>
#include<hip/hip_runtime.h>
#include<hip/hip_runtime_api.h>

#define ITER (1<<16)

template<typename T>
__global__ void Kernel(T* Out, T *In) {
    int tx = hipThreadIdx_x;
    T in = In[tx];
    T out = Out[tx];
    for(int i=0;i<ITER;i++) {
        asm volatile(  "v_add_f32 %0, %1, %1 row_shr:1 bound_ctrl:0 \n \
                        v_add_f32 %0, %1, %0 row_shr:2 bound_ctrl:0 \n \
                        v_add_f32 %0, %1, %0 row_shr:3 bound_ctrl:0 \n \
                        v_nop \n \
                        v_nop \n \
                        v_add_f32 %0, %0, %0 row_shr:4 bank_mask:0xe \n \
                        v_nop \n \
                        v_nop \n \
                        v_add_f32 %0, %0, %0 row_shr:8 bank_mask:0xc \n \
                        v_nop \n \
                        v_nop \n \
                        v_add_f32 %0, %0, %0 row_bcast:15 row_mask:0xa \n \
                        v_nop \n \
                        v_nop \n \
                        v_add_f32 %0, %0, %0 row_bcast:31 row_mask:0xc \n" : "=v"(out) : "v"(in + out));
    }
    Out[tx] = out;
}

typedef float __float4 __attribute__((ext_vector_type(4)));

__global__ void Kernelx4(__float4* Out, __float4 *In) {
    int tx = hipThreadIdx_x;
    __float4 in = In[tx];
    __float4 out = Out[tx];
    for(int i=0;i<ITER;i++) {
        asm volatile(  "v_add_f32 %0, %1, %1 row_shr:1 bound_ctrl:0 \n \
                        v_add_f32 %0, %1, %0 row_shr:2 bound_ctrl:0 \n \
                        v_add_f32 %0, %1, %0 row_shr:3 bound_ctrl:0 \n \
                        \n \
                        v_add_f32 %2, %3, %3 row_shr:1 bound_ctrl:0 \n \
                        v_add_f32 %2, %3, %2 row_shr:2 bound_ctrl:0 \n \
                        v_add_f32 %2, %3, %2 row_shr:3 bound_ctrl:0 \n \
                        \n \
                        v_add_f32 %4, %5, %5 row_shr:1 bound_ctrl:0 \n \
                        v_add_f32 %4, %5, %4 row_shr:2 bound_ctrl:0 \n \
                        v_add_f32 %4, %5, %4 row_shr:3 bound_ctrl:0 \n \
                        \n \
                        v_add_f32 %6, %7, %7 row_shr:1 bound_ctrl:0 \n \
                        v_add_f32 %6, %7, %6 row_shr:2 bound_ctrl:0 \n \
                        v_add_f32 %6, %7, %6 row_shr:3 bound_ctrl:0 \n \
                        \n \
                        v_add_f32 %0, %0, %0 row_shr:4 bank_mask:0xe \n \
                        \n \
                        v_add_f32 %2, %2, %2 row_shr:4 bank_mask:0xe \n \
                        \n \
                        v_add_f32 %4, %4, %4 row_shr:4 bank_mask:0xe \n \
                        \n \
                        v_add_f32 %6, %6, %6 row_shr:4 bank_mask:0xe \n \
                        \n \
                        v_add_f32 %0, %0, %0 row_shr:8 bank_mask:0xc \n \
                        \n \
                        v_add_f32 %2, %2, %2 row_shr:8 bank_mask:0xc \n \
                        \n \
                        v_add_f32 %4, %4, %4 row_shr:8 bank_mask:0xc \n \
                        \n \
                        v_add_f32 %6, %6, %6 row_shr:8 bank_mask:0xc \n \
                        \n \
                        v_add_f32 %0, %0, %0 row_bcast:15 row_mask:0xa \n \
                        \n \
                        v_add_f32 %2, %2, %2 row_bcast:15 row_mask:0xa \n \
                        \n \
                        v_add_f32 %4, %4, %4 row_bcast:15 row_mask:0xa \n \
                        \n \
                        v_add_f32 %6, %6, %6 row_bcast:15 row_mask:0xa \n \
                        \n \
                        v_add_f32 %0, %0, %0 row_bcast:31 row_mask:0xc \n \
                        \n \
                        v_add_f32 %2, %2, %2 row_bcast:31 row_mask:0xc \n \
                        \n \
                        v_add_f32 %4, %4, %4 row_bcast:31 row_mask:0xc \n \
                        \n \
                        v_add_f32 %6, %6, %6 row_bcast:31 row_mask:0xc \n \
                        " :  : "v"(out.x),"v"(in.x + out.x),"v"(out.y), "v"(in.y + out.y), "v"(out.y), "v"(in.y+out.y), "v"(out.z), "v"(out.z+in.z), "v"(out.w), "v"(out.w+in.w));
    }
    Out[tx] = out;
}


typedef float U;
#define LEN 1024
#define SIZE (LEN * sizeof(U))

int main() {
    U *hOut, *hIn, *dOut, *dIn;
    hIn = new U[LEN];
    hOut = new U[LEN];
    for(int i=0;i<LEN;i++) {
        hIn[i] = 1.0f;
        hOut[i] = 0.0f;
    }
    constexpr size_t wg = 64*40;

    hipMalloc(&dOut, SIZE);
    hipMalloc(&dIn, SIZE);

    hipMemcpy(dIn, hIn, SIZE, hipMemcpyHostToDevice);
    hipMemcpy(dOut, hOut, SIZE, hipMemcpyHostToDevice);

    auto start = std::chrono::high_resolution_clock::now();
    hipLaunchKernelGGL(Kernel, dim3(wg,1,1), dim3(LEN,1,1), 0, 0, dOut, dIn);
    hipDeviceSynchronize();
    auto stop = std::chrono::high_resolution_clock::now();

    double elapsedSec = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();

    double tp = (double)(wg*ITER*LEN*64) / 1.0E12 / elapsedSec;

    std::cout<<tp<<std::endl;

    hipMemcpy(dIn, hIn, SIZE, hipMemcpyHostToDevice);
    hipMemcpy(dOut, hOut, SIZE, hipMemcpyHostToDevice);

    start = std::chrono::high_resolution_clock::now();
    hipLaunchKernelGGL(Kernelx4, dim3(wg,1,1), dim3(LEN/4,1,1), 0, 0, reinterpret_cast<__float4*>(dOut), reinterpret_cast<__float4*>(dIn));
    hipDeviceSynchronize();
    stop = std::chrono::high_resolution_clock::now();

    elapsedSec = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();

    tp = (double)(wg*ITER*LEN*64) / 1.0E12 / elapsedSec;

    std::cout<<tp<<std::endl;


}
