#include<hip_runtime.h>
#include<hip_runtime_api.h>
#include<iostream>
#include<cstdint>

#define LEN 64
#define SIZE LEN*sizeof(uint32_t)

#define kernelName "hello_world"
#define fileName "gather.co"

int main()
{
  uint32_t *In, *Index, *Out ;
  hipDeviceptr_t Ind, Indexd, Outd;
  In = new uint32_t[LEN];
  Index = new uint32_t[LEN];
  Out = new uint32_t[LEN];

  for(uint32_t i=0;i<LEN;i++)
  {
    In[i] = i;
    Index[i] = i>31 ? i-32 : 0;
    Out[i] = 0;
  }

  hipInit(0);
  hipDevice_t device;
  hipCtx_t context;
  hipDeviceGet(&device, 0);
  hipCtxCreate(&context, 0, device);

  hipMalloc((void**)&Ind, SIZE);
  hipMalloc((void**)&Indexd, SIZE);
  hipMalloc((void**)&Outd, SIZE);

  hipMemcpyHtoD(Ind, In, SIZE);
  hipMemcpyHtoD(Indexd, Index, SIZE);
  hipMemcpyHtoD(Outd, Out, SIZE);

  hipModule_t Module;
  hipFunction_t Function;

  hipModuleLoad(&Module, fileName);
  hipModuleGetFunction(&Function, Module, kernelName);

  std::vector<void*>argBuffer(3);
  memcpy(&argBuffer[0], &Ind, sizeof(void*));
  memcpy(&argBuffer[1], &Indexd, sizeof(void*));
  memcpy(&argBuffer[2], &Outd, sizeof(void*));

  size_t size = argBuffer.size() * sizeof(void*);

  void *config[] = {
    HIP_LAUNCH_PARAM_BUFFER_POINTER, &argBuffer[0],
    HIP_LAUNCH_PARAM_BUFFER_SIZE, &size,
    HIP_LAUNCH_PARAM_END
  };

  hipModuleLaunchKernel(Function, 1, 1, 1, LEN, 1, 1, 0, 0, NULL, (void**)&config);

  hipMemcpyDtoH(Out, Outd, SIZE);
  for(uint32_t i=0;i<LEN/2;i++)
  {
	  assert(Out[i] == In[i + 32]);
  }
  std::cout<<"Passed!"<<std::endl;
}
