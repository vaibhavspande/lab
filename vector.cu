#include <stdio.h>
// CUDA kernel for vector addition
__global__ void vectorAdd(int *a, int *b, int *c, int size)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < size)
    {
        c[tid] = a[tid] + b[tid];
    }
}

int main()
{
    int size = 1000000;             // Size of the vectors
    int *a, *b, *c;             // Host vectors
    int *dev_a, *dev_b, *dev_c; // Device vectors
    // Allocate memory for host vectors
    a = (int *)malloc(size * sizeof(int));
    b = (int *)malloc(size * sizeof(int));
    c = (int *)malloc(size * sizeof(int));
    // Initialize host vectors
    for (int i = 0; i < size; i++)
    {
        a[i] = i;
        b[i] = 2 * i;
    }
    // Allocate memory on the device for device vectors
    cudaMalloc( (void **)&dev_a, size * sizeof(int));
    cudaMalloc((void **)&dev_b, size * sizeof(int));
    cudaMalloc((void **)&dev_c, size * sizeof(int));
    // Copy host vectors to device
    cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
    // Launch kernel for vector addition
    int blockSize = 256;
    int gridSize = (size + blockSize - 1) / blockSize;

    vectorAdd<<<gridSize, blockSize>>>(dev_a, dev_b, dev_c, size);
    // Copy result from device to host
    cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);

    clock_t start = clock();
    for (int i = 0; i < size; ++i)
    {
        c[i] = a[i] + b[i];
    }
    clock_t end = clock();
    double sequentialTime = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Measure performance of parallel algorithm
    cudaEvent_t parallelStart, parallelEnd;
    cudaEventCreate(&parallelStart);
    cudaEventCreate(&parallelEnd);
    cudaEventRecord(parallelStart);

    vectorAdd<<<gridSize, blockSize>>>(dev_a, dev_b, dev_c, size);
    cudaEventRecord(parallelEnd);
    cudaEventSynchronize(parallelEnd);
    float parallelTime = 0;
    cudaEventElapsedTime(&parallelTime, parallelStart, parallelEnd);

    parallelTime /= 1000; // Convert milliseconds to seconds



    printf("Sequential Time: %f seconds\n", sequentialTime);
    printf("Parallel Time: %f seconds\n", parallelTime);


    // Free device memory
    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);
    // Free host memory
    free(a);
    free(b);
    free(c);

    return 0;
}
