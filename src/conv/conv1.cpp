/**
Convolution on 4x4 weights with 16x16 image
*/

#include<iostream>
#include<hip/hip_runtime.h>
#include<hip/hip_runtime_api.h>

static constexpr size_t wLen = 4*4;
static constexpr size_t iLen = 16*16;

__global__ void Kernel(float *wImage, float *inImage, float *outImage) {
    int tx = hipThreadIdx_x;
    float in = inImage[tx];
    float out = outImage[tx];
    out += in * wImage[0];
    out += in * wImage[1];
    out += in * wImage[2];
    out += in * wImage[3];
    outImage[tx] = out;
}

int main() {
    float *hwImage, *hinImage, *houtImage, *dwImage, *dinImage, *doutImage;
    hwImage = new float[wLen];
    hinImage = new float[iLen];
    houtImage = new float[iLen];

    for(int i=0;i<wLen;i++) {
        hwImage[i] = 1.0f + i;
    }

    for(int i=0;i<iLen;i++) {
        hinImage[i] = 1.0f * i;
        houtImage[i] = 0.0f;
    }

    hipMalloc(&dwImage, wLen*sizeof(float));
    hipMalloc(&dinImage, iLen*sizeof(float));
    hipMalloc(&doutImage, iLen*sizeof(float));

    hipMemcpy(dwImage, hwImage, wLen*sizeof(float), hipMemcpyHostToDevice);
    hipMemcpy(dinImage, hinImage, iLen*sizeof(float), hipMemcpyHostToDevice);
    hipMemcpy(doutImage, houtImage, iLen*sizeof(float), hipMemcpyHostToDevice);

    hipLaunchKernelGGL(Kernel, dim3(1,1,1), dim3(16,1,1), 0, 0, dwImage, dinImage, doutImage);
    hipDeviceSynchronize();

    hipMemcpy(houtImage, doutImage, iLen*sizeof(float), hipMemcpyDeviceToHost);
    for(int i=0;i<16;i++) {
        std::cout<<houtImage[i]<<std::endl;
    }
}
