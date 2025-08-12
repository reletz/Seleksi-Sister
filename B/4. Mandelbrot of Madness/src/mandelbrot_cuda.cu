#include "mandelbrot.h"

#define CUDA_CHECK(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line) {
	if (code != cudaSuccess) {
		fprintf(stderr,"CUDA Error: %s %s %d\n", cudaGetErrorString(code), file, line);
		exit(code);
	}
}

// setiap thread GPU
__device__ double mandelbrot_gpu(double cr, double ci) {
	double zr = 0.0, zi = 0.0;
	int iter = 0;

	// interior (Cardioid & Period-2 bulb)
	double ci2 = ci * ci;
	double q = (cr - 0.25) * (cr - 0.25) + ci2;
	if (q * (q + (cr - 0.25)) <= 0.25 * ci2 ||
	(cr + 1.0) * (cr + 1.0) + ci2 <= 0.0625) {
		return (double)MAX_ITER;
	}

	double zr2 = 0.0, zi2 = 0.0;
	while (iter < MAX_ITER) {
	zr2 = zr * zr;
	zi2 = zi * zi;

	if (zr2 + zi2 > 4.0) {
		double log_zn = log(zr2 + zi2) / 2.0;
		double nu = log(log_zn / log(2.0)) / log(2.0);
		return (double)iter + 1.0 - nu;
	}

	zi = 2.0 * zr * zi + ci;
	zr = zr2 - zi2 + cr;
	++iter;
	}

	return (double)MAX_ITER;
}

// Kernel utama
__global__ void mandelbrot_kernel(unsigned char* image) {
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;

	if (x < WIDTH && y < HEIGHT) {
		double cr = X_MIN + (X_MAX - X_MIN) * x / (WIDTH - 1);
		double ci = Y_MIN + (Y_MAX - Y_MIN) * y / (HEIGHT - 1);

		double iter = mandelbrot_gpu(cr, ci);

		int idx = CHANNELS * (y * WIDTH + x);

		double t = 0.1 * iter;
		double t2 = t * t;
		double t3 = t2 * t;
		double one_minus_t = 1.0 - t;
		double one_minus_t2 = one_minus_t * one_minus_t;
		double one_minus_t3 = one_minus_t2 * one_minus_t;

		image[idx + 0] = (unsigned char)((iter >= MAX_ITER) ? 0 : (9.0 * one_minus_t * t3 * 255.0));
		image[idx + 1] = (unsigned char)((iter >= MAX_ITER) ? 0 : (15.0 * one_minus_t2 * t2 * 255.0));
		image[idx + 2] = (unsigned char)((iter >= MAX_ITER) ? 0 : (8.5 * one_minus_t3 * t * 255.0));
	}
}

extern "C" std::chrono::milliseconds mandelbrot_cuda(unsigned char* h_image) {
  const int image_size = WIDTH * HEIGHT * CHANNELS * sizeof(unsigned char);
	
	unsigned char* d_image;
	cudaMalloc(&d_image, image_size);

	dim3 threadsPerBlock(16, 16);
	dim3 numBlocks((WIDTH + 15) / 16, (HEIGHT + 15) / 16);

	auto start_time = std::chrono::high_resolution_clock::now();

	CUDA_CHECK(cudaMalloc(&d_image, image_size));
	mandelbrot_kernel<<<numBlocks, threadsPerBlock>>>(d_image);
	CUDA_CHECK(cudaGetLastError());
	CUDA_CHECK(cudaDeviceSynchronize());
	CUDA_CHECK(cudaMemcpy(h_image, d_image, image_size, cudaMemcpyDeviceToHost));
	CUDA_CHECK(cudaFree(d_image));

	cudaDeviceSynchronize();

	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

	cudaMemcpy(h_image, d_image, image_size, cudaMemcpyDeviceToHost);
	
	cudaFree(d_image);
	return duration;
}