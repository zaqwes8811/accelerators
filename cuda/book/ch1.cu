#include "external/book-professional-cuda-c-programming/CodeSamples/common/common.h"

#include <cuda_runtime.h>

#include <iostream>

#include <stdio.h>

static void info() {
	// http://developer.download.nvidia.com/compute/cuda/2_3/toolkit/docs/online/structcudaDeviceProp.html
	int nDevices;

	cudaGetDeviceCount(&nDevices);
	for (int i = 0; i < nDevices; i++) {
		cudaDeviceProp prop;
		cudaGetDeviceProperties(&prop, i);
		printf("Device Number: %d\n", i);
		printf("  Device name: %s\n", prop.name);
		printf("  Memory Clock Rate (KHz): %d\n", prop.memoryClockRate);
		printf("  Memory Bus Width (bits): %d\n", prop.memoryBusWidth);
		printf("  Total Global Memory (MBytes): %zu\n",
				prop.totalGlobalMem / 1024 / 1024);
		printf("  Warp Size: %d\n", prop.warpSize);
		printf("  Peak Memory Bandwidth (GB/s): %f\n\n",
				2.0 * prop.memoryClockRate * (prop.memoryBusWidth / 8) / 1.0e6);
	}
}

/*
 * This example demonstrates a simple vector sum on the GPU and on the host.
 * sumArraysOnGPU splits the work of the vector sum across CUDA threads on the
 * GPU. A 1D thread block and 1D grid are used. sumArraysOnHost sequentially
 * iterates through vector elements on the host.
 */

void initialData(float *ip, const int size) {
	int i;

	for (i = 0; i < size; i++) {
		ip[i] = (float) (rand() & 0xFF) / 10.0f;
	}

	return;
}

void sumMatrixOnHost(float *A, float *B, float *C, const int nx, const int ny) {
	float *ia = A;
	float *ib = B;
	float *ic = C;

	for (int iy = 0; iy < ny; iy++) {
		for (int ix = 0; ix < nx; ix++) {
			ic[ix] = ia[ix] + ib[ix];

		}

		ia += nx;
		ib += nx;
		ic += nx;
	}

	return;
}

void checkResult(float *hostRef, float *gpuRef, const int N) {
	double epsilon = 1.0E-8;
	bool match = 1;

	for (int i = 0; i < N; i++) {
		if (abs(hostRef[i] - gpuRef[i]) > epsilon) {
			match = 0;
			printf("host %f gpu %f\n", hostRef[i], gpuRef[i]);
			break;
		}
	}

	if (match)
		printf("Arrays match.\n\n");
	else
		printf("Arrays do not match.\n\n");
}

// grid 1D block 1D
__global__ void sumMatrixOnGPU1D(float *MatA, float *MatB, float *MatC, int nx,
		int ny) {
	unsigned int ix = threadIdx.x + blockIdx.x * blockDim.x;

	if (ix < nx)
		for (int iy = 0; iy < ny; iy++) {
			int idx = iy * nx + ix;
			MatC[idx] = MatA[idx] + MatB[idx];
		}

}

int to_mb(int bytes) {
	return bytes / 1024 / 1024;
}

int main(int argc, char **argv) {
	printf("%s Starting...\n", argv[0]);

	info();

	size_t mem_tot_0 = 0;
	size_t mem_free_0 = 0;
//	size_t mem_tot_1 = 0;
//	size_t mem_free_1 = 0;
//	// out of mem
//	cudaSetDevice(0);
//	cudaDeviceSynchronize();
//	cudaThreadSynchronize();

	cudaDeviceReset();
	cudaMemGetInfo(&mem_free_0, &mem_tot_0);
	std::cout << "Free memory before copy dev 0: " << mem_free_0 / 1024 / 1024
			<< std::endl;

	// set up device
	int dev = 0;
	cudaDeviceProp deviceProp;
	CHECK(cudaGetDeviceProperties(&deviceProp, dev));
	printf("Using Device %d: %s\n", dev, deviceProp.name);
	CHECK(cudaSetDevice(dev));

	// set up data size of matrix
	int nx = 1 << 12; //14;
	int ny = 1 << 12; // 14;

	int nxy = nx * ny;
	int nBytes = nxy * sizeof(float);
	printf("Matrix size: nx %d ny %d nMBytes %d\n", nx, ny, to_mb(nBytes));

	// malloc host memory
	float *h_A, *h_B, *hostRef, *gpuRef;
	h_A = (float *) malloc(nBytes);
	h_B = (float *) malloc(nBytes);
	hostRef = (float *) malloc(nBytes);
	gpuRef = (float *) malloc(nBytes);

	// initialize data at host side
	double iStart = seconds();
	initialData(h_A, nxy);
	initialData(h_B, nxy);
	double iElaps = seconds() - iStart;
	printf("initialize matrix elapsed %f sec\n", iElaps);

	memset(hostRef, 0, nBytes);
	memset(gpuRef, 0, nBytes);

	// add matrix at host side for result checks
	iStart = seconds();
	sumMatrixOnHost(h_A, h_B, hostRef, nx, ny);
	iElaps = seconds() - iStart;
	printf("sumMatrixOnHost elapsed %f sec\n", iElaps);

	// malloc device global memory
	float *d_MatA, *d_MatB, *d_MatC;
	CHECK(cudaMalloc((void ** )&d_MatA, nBytes));
	CHECK(cudaMalloc((void ** )&d_MatB, nBytes));
	CHECK(cudaMalloc((void ** )&d_MatC, nBytes));

	// transfer data from host to device
	CHECK(cudaMemcpy(d_MatA, h_A, nBytes, cudaMemcpyHostToDevice));
	CHECK(cudaMemcpy(d_MatB, h_B, nBytes, cudaMemcpyHostToDevice));

	// invoke kernel at host side
	int dimx = 32;
	dim3 block(dimx, 1);
	dim3 grid((nx + block.x - 1) / block.x, 1);

	iStart = seconds();
	sumMatrixOnGPU1D<<<grid, block>>>(d_MatA, d_MatB, d_MatC, nx, ny);
	CHECK(cudaDeviceSynchronize());
	iElaps = seconds() - iStart;
	printf("sumMatrixOnGPU1D <<<(%d,%d), (%d,%d)>>> elapsed %f sec\n", grid.x,
			grid.y, block.x, block.y, iElaps);

	// check kernel error
	CHECK(cudaGetLastError());

	// copy kernel result back to host side
	CHECK(cudaMemcpy(gpuRef, d_MatC, nBytes, cudaMemcpyDeviceToHost));

	// check device results
	checkResult(hostRef, gpuRef, nxy);

	// free device global memory
	CHECK(cudaFree(d_MatA));
	CHECK(cudaFree(d_MatB));
	CHECK(cudaFree(d_MatC));

	// free host memory
	free(h_A);
	free(h_B);
	free(hostRef);
	free(gpuRef);

	// reset device
	CHECK(cudaDeviceReset());

	return (0);
}
