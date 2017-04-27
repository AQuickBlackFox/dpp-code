#include<hip/hip_runtime.h>
#include<hip/hip_runtime_api.h>
#include<iostream>
#include<cstdint>

#define X 4
#define Y 4
#define LEN X*Y
#define SIZE LEN*sizeof(float)

#define kernelName "Run"
#define fileName "mat.co"

int main()
{
  float *vertex, *transform, *output ;
  hipDeviceptr_t vertex_d, transform_d, output_d;
  vertex = new float[X];
  output = new float[Y];
  transform = new float[LEN];

  for(uint32_t i=0;i<LEN;i++)
  {
    transform[i] = i;
  }

  for(uint32_t i=0;i<X;i++)
  {
    vertex[i] = 1.0f;
  }

  for(uint32_t i=0;i<Y;i++)
  {
    output[i] = 0.0f;
  }

  hipInit(0);
  hipDevice_t device;
  hipCtx_t context;
  hipDeviceGet(&device, 0);
  hipCtxCreate(&context, 0, device);

  hipMalloc((void**)&vertex_d, X*sizeof(float));
  hipMalloc((void**)&transform_d, LEN*sizeof(float));
  hipMalloc((void**)&output_d, Y*sizeof(float));

  hipMemcpyHtoD(vertex_d, vertex, X*sizeof(float));
  hipMemcpyHtoD(transform_d, transform, LEN*sizeof(float));
  hipMemcpyHtoD(output_d, output, Y*sizeof(float));

  hipModule_t Module;
  hipFunction_t Function;

  hipModuleLoad(&Module, fileName);
  hipModuleGetFunction(&Function, Module, kernelName);

  std::vector<void*>argBuffer(3);
  memcpy(&argBuffer[0], &vertex_d, sizeof(void*));
  memcpy(&argBuffer[1], &transform_d, sizeof(void*));
  memcpy(&argBuffer[2], &output_d, sizeof(void*));

  size_t size = argBuffer.size() * sizeof(void*);

  void *config[] = {
    HIP_LAUNCH_PARAM_BUFFER_POINTER, &argBuffer[0],
    HIP_LAUNCH_PARAM_BUFFER_SIZE, &size,
    HIP_LAUNCH_PARAM_END
  };

  hipModuleLaunchKernel(Function, 1, 1, 1, X, 1, 1, 0, 0, NULL, (void**)&config);

  hipMemcpyDtoH(output, output_d, SIZE);

  hipFree(vertex_d);
  hipFree(transform_d);
  hipFree(output_d);


  for(int i=0;i<Y;i++)
  {
    std::cout<<output[i]<<" ";
  }
  std::cout<<std::endl;

  delete vertex;
  delete transform;
  delete output;

  std::cout<<"done"<<std::endl;
}
