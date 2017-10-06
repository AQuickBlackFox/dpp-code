#include<iostream>
#include<hip/hip_runtime_api.h>

#define HIP_CHECK(status) \
if(status != hipSuccess) {std::cout<<"Got Status: "<<status<<" at Line: "<<__LINE__<<std::endl;exit(0);}

#define LEN 16*16
#define SIZE (LEN*sizeof(float))

#define fileName "conv1.co"
#define kernelName "hello_world"

int main() {
    float *wImage, *inImage, *outImage;
    hipDeviceptr_t dwImage, dinImage, doutImage;

    wImage = new float[LEN];
    inImage = new float[LEN];
    outImage = new float[LEN];

    for(int i=0;i<LEN;i++) {
        wImage[i] = 1.0f + i;
        inImage[i] = 1.0f * i;
        outImage[i] = 0.0f;
    }

    HIP_CHECK(hipInit(0));
    hipDevice_t device;
    hipCtx_t context;
    HIP_CHECK(hipDeviceGet(&device, 0));
    HIP_CHECK(hipCtxCreate(&context, 0, device));

    HIP_CHECK(hipMalloc((void**)&dwImage, SIZE));
    HIP_CHECK(hipMalloc((void**)&dinImage, SIZE));
    HIP_CHECK(hipMalloc((void**)&doutImage, SIZE));

    HIP_CHECK(hipMemcpyHtoD(dwImage, wImage, SIZE));
    HIP_CHECK(hipMemcpyHtoD(dinImage, inImage, SIZE));
    HIP_CHECK(hipMemcpyHtoD(doutImage, outImage, SIZE));

    hipModule_t Module;
    hipFunction_t Function;

    HIP_CHECK(hipModuleLoad(&Module, fileName));
    HIP_CHECK(hipModuleGetFunction(&Function, Module, kernelName));

    struct {
        void *w;
        void *in;
        void *out;
    } args;

    args.w = dwImage;
    args.in = dinImage;
    args.out = doutImage;

    size_t size = sizeof(args);

    void *config[] = {
        HIP_LAUNCH_PARAM_BUFFER_POINTER, &args,
        HIP_LAUNCH_PARAM_BUFFER_SIZE, &size,
        HIP_LAUNCH_PARAM_END
    };

    HIP_CHECK(hipModuleLaunchKernel(Function, 1,1,1, 16,1,1, 0,0, NULL, (void**)&config));

    HIP_CHECK(hipMemcpyDtoH(outImage, doutImage, SIZE));

    for(int i=0;i<16;i++) {
        std::cout<<outImage[i]<<" ";
    }
    std::cout<<std::endl;

}
