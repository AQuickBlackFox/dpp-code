#include<iostream>
#include<hip/hip_runtime_api.h>
#include<hip/hip_runtime.h>

// v_mac_f32 v0, v1, v2 quad_perm: [0,0,0,0]
extern "C" __device__ float __mac_qp0(float, float, float);
// v_mac_f32 v0, v1, v2 quad_perm: [1,1,1,1]
extern "C" __device__ float __mac_qp1(float, float, float);
// v_mac_f32 v0, v1, v2 quad_perm: [2,2,2,2]
extern "C" __device__ float __mac_qp2(float, float, float);
// v_mac_f32 v0, v1, v2 quad_perm: [3,3,3,3]
extern "C" __device__ float __mac_qp3(float, float, float);

#define AY 1
#define AX 4
#define BX 64
#define BY AX
#define CX BX
#define CY AY
/*
Kernel for doing matrix multiplication
C = A * B
(1x64) = (1x4) * (4x64)
*/

/*
The implementation is different than usual mat mul on GPU
because we are trying to use DPPs.
TODO: Take an optimized mat mul and use dpps there.
TODO: Mat Mul should scale for different sizes.
DDPs are used to decrease LDS contention by factor of 4.
*/

template<int Ax>
__global__ void MMul(hipLaunchParm lp, float *A, float *B, float *C){
  int tx = hipThreadIdx_x;
  /*
  Create LDS size equal to number of floats
  as width of A
  */
  __shared__ float As[Ax];

  /*
  Load matrix A into LDS.
  As Ax < number of threads, we have to load
  through conditional. (Should not create GPUVM faults)
  */
  if(tx < Ax){
    As[tx] = A[tx];
  }

  /*
  a is to load data from LDS
  */
  float a;

  /*
  b0, b1, b2, b3 are for loading B matrix
  */
  float b0, b1, b2, b3;
  b0 = B[tx+0*BX];
  b1 = B[tx+1*BX];
  b2 = B[tx+2*BX];
  b3 = B[tx+3*BX];

  /*
  c contains output
  c0, c1, c2, c3 contains output from
  mac quad permute.
  This approach is better than mov quad_perm and then mac
  */
  float c, c0, c1, c2, c3;
  c0 = 0.0f;

  /*
  Load first 4 elements of LDS memory
  a = As[0],As[1],As[2],As[3],As[0]....
  */
  a = As[tx%4];

  /*
  c1 = a * b0 + c0;
  c2 = a * b1 + c1;
  c3 = a * b2 + c2;
  c  = a * b3 + c3;
  */
  c1 = __mac_qp0(c0, a, b0);
  c2 = __mac_qp1(c1, a, b1);
  c3 = __mac_qp2(c2, a, b2);
  c  = __mac_qp3(c3, a, b3);

  /*
  Store output to matrix
  */
  C[tx] = c;
}

int main(){
  float *A = new float[AX*AY];
  float *B = new float[BX*BY];
  float *C = new float[CX*CY];

  for(int j=0;j<AY;j++){
    for(int i=0;i<AX;i++){
      A[i+j*AX] = 1.0f*i;
    }
  }

  for(int j=0;j<BY;j++){
    for(int i=0;i<BX;i++){
      B[i+j*BX] = 1.0f;
    }
  }

  for(int j=0;j<CY;j++){
    for(int i=0;i<CX;i++){
      C[i+j*CX] = 0.0f;
    }
  }

  float *Ad, *Bd, *Cd;
  hipMalloc(&Ad, AX*AY*sizeof(float));
  hipMalloc(&Bd, BX*BY*sizeof(float));
  hipMalloc(&Cd, CX*CY*sizeof(float));
  hipMemcpy(Ad, A, AX*AY*sizeof(float), hipMemcpyHostToDevice);
  hipMemcpy(Bd, B, BX*BY*sizeof(float), hipMemcpyHostToDevice);
  hipMemcpy(Cd, C, CX*CY*sizeof(float), hipMemcpyHostToDevice);

  hipLaunchKernel((MMul<AX>), dim3(1,1,1), dim3(BX,1,1), 0, 0, Ad, Bd, Cd);

  hipMemcpy(C, Cd, CX*CY*sizeof(float), hipMemcpyDeviceToHost);
  for(int i=0;i<CX;i++){
    std::cout<<C[i]<<std::endl;
  }
}
