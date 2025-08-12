#include "mandelbrot.h"

double mandelbrot(double cr, double ci) {
  double zr = 0.0, zi = 0.0;
  int iter = 0;

  // interior (Cardioid & Period-2 bulb)
  double ci2 = ci * ci;
  double q = (cr - 0.25) * (cr - 0.25) + ci2;
  if (q * (q + (cr - 0.25)) <= 0.25 * ci2 ||
    (cr + 1.0) * (cr + 1.0) + ci2 <= 0.0625) {
    return (double)MAX_ITER;
  }

  // regular mandelbrot
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