#ifndef _MANDELBROT_H_
#define _MANDELBROT_H_

#include <iostream>
#include <chrono>
#include <cmath>
#include <complex>
#include <vector>
#include <omp.h>

// Image parameters
const int WIDTH = 3840;
const int HEIGHT = 2160;
const int MAX_ITER = 10000;
const int CHANNELS = 3;

// Mandelbrot region
const double X_MIN = -2.0;
const double X_MAX = 1.0;
const double Y_MIN = -1.0;
const double Y_MAX = 1.0;

double mandelbrot(double cr, double ci);
void color_pixel(unsigned char* image, int x, int y, double iter);

#ifdef __cplusplus
extern "C" {
#endif

std::chrono::milliseconds mandelbrot_cuda(unsigned char* h_image);

#ifdef __cplusplus
}
#endif

#endif