#include <stdio.h>

__global__ void compute() {
    printf("Running on RTX 3060, thread %d\n", threadIdx.x);
}

int main() {
    compute<<<1, 8>>>();
    cudaDeviceSynchronize();
    return 0;
}
