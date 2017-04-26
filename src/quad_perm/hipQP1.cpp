#include<hip/hip_runtime.h>
#include<hip/hip_runtime_api.h>
#include<iostream>
#include<cstdint>

#define LEN 64
#define SIZE LEN*sizeof(uint32_t)

#define kernelName "Run"
#define fileName "qp1.co"

int main()
{
  uint32_t *In, *Out ;
  hipDeviceptr_t Ind, Outd;
  In = new uint32_t[LEN];
  Out = new uint32_t[LEN];

  for(uint32_t i=0;i<LEN;i++)
  {
    In[i] = i;
    Out[i] = 0;
  }

  hipInit(0);
  hipDevice_t device;
  hipCtx_t context;
  hipDeviceGet(&device, 0);
  hipCtxCreate(&context, 0, device);

  hipMalloc((void**)&Ind, SIZE);
  hipMalloc((void**)&Outd, SIZE);

  hipMemcpyHtoD(Ind, In, SIZE);
  hipMemcpyHtoD(Outd, Out, SIZE);

  hipModule_t Module;
  hipFunction_t Function;

  hipModuleLoad(&Module, fileName);
  hipModuleGetFunction(&Function, Module, kernelName);

  std::vector<void*>argBuffer(2);
  memcpy(&argBuffer[0], &Ind, sizeof(void*));
  memcpy(&argBuffer[1], &Outd, sizeof(void*));

  size_t size = argBuffer.size() * sizeof(void*);

  void *config[] = {
    HIP_LAUNCH_PARAM_BUFFER_POINTER, &argBuffer[0],
    HIP_LAUNCH_PARAM_BUFFER_SIZE, &size,
    HIP_LAUNCH_PARAM_END
  };

  hipModuleLaunchKernel(Function, 1, 1, 1, LEN, 1, 1, 0, 0, NULL, (void**)&config);

  hipMemcpyDtoH(Out, Outd, SIZE);

  for(int j=0;j<16;j++){
    for(int i=0;i<4;i++){
      std::cout<<In[i+j*4]<<" ";
    }
    std::cout<<std::endl;
  }

  std::cout<<std::endl;

  for(int j=0;j<16;j++){
    for(int i=0;i<4;i++){
      std::cout<<Out[i+j*4]<<" ";
    }
    std::cout<<std::endl;
  }

  std::cout<<"Passed!"<<std::endl;

}
