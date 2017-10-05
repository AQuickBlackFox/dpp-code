
typedef float float4 __attribute__((ext_vector_type(4)));

__global__ void Kernel(float4 *wImage, float *inImage, float *outImage) {
    int tx = hipThreadIdx_x;
    float4 a = wImage[0];
    float4 b = wImage[1];
    float4 c = wImage[2];
    float4 d = wImage[3];
        
}
