#include "mandelbrot.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void color_pixel(unsigned char* image, int x, int y, double iter) {
  int idx = CHANNELS * (y * WIDTH + x);

  if (iter >= MAX_ITER) {
    image[idx + 0] = 0;
    image[idx + 1] = 0;
    image[idx + 2] = 0;
  } else {
    double t = 0.1 * iter;
    double t2 = t * t;
    double t3 = t2 * t;
    double one_minus_t = 1.0 - t;
    double one_minus_t2 = one_minus_t * one_minus_t;
    double one_minus_t3 = one_minus_t2 * one_minus_t;

    image[idx + 0] = (unsigned char)(9.0 * one_minus_t * t3 * 255.0);   // Merah
    image[idx + 1] = (unsigned char)(15.0 * one_minus_t2 * t2 * 255.0); // Hijau
    image[idx + 2] = (unsigned char)(8.5 * one_minus_t3 * t * 255.0);   // Biru
  }
}

auto serial(){
  unsigned char *image = new unsigned char[WIDTH * HEIGHT * CHANNELS];

  std::cout << "Generating Mandelbrot set (" << WIDTH << "x" << HEIGHT << ") serially..." << std::endl;

  auto start_time = std::chrono::high_resolution_clock::now();

  for (int y = 0; y < HEIGHT; ++y){
    for (int x = 0; x < WIDTH; ++x){
      double cr = X_MIN + (X_MAX - X_MIN) * x / (WIDTH - 1);
      double ci = Y_MIN + (Y_MAX - Y_MIN) * y / (HEIGHT - 1);
      
      double iter = mandelbrot(cr, ci);
      color_pixel(image, x, y, iter);
    }
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

  std::cout << "Generation of serial finished in " << duration.count() << " ms." << std::endl;

  // file PNG
  if (stbi_write_png("output/mandelbrot_serial.png", WIDTH, HEIGHT, CHANNELS, image, WIDTH * CHANNELS)) {
    std::cout << "Image saved to mandelbrot_serial.png" << std::endl;
  } else {
    std::cerr << "Error saving image." << std::endl;
  }

  delete[] image;
  return duration;
}

auto parallel(){
  unsigned char *image = new unsigned char[WIDTH * HEIGHT * CHANNELS];

  std::cout << "Generating Mandelbrot set (" << WIDTH << "x" << HEIGHT << ")with parallel..." << std::endl;

  auto start_time = std::chrono::high_resolution_clock::now();

  #pragma omp parallel for schedule(dynamic)
  for (int y = 0; y < HEIGHT; ++y){
    for (int x = 0; x < WIDTH; ++x){
      double cr = X_MIN + (X_MAX - X_MIN) * x / (WIDTH - 1);
      double ci = Y_MIN + (Y_MAX - Y_MIN) * y / (HEIGHT - 1);
      
      double iter = mandelbrot(cr, ci);
      color_pixel(image, x, y, iter);
    }
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

  std::cout << "Generation of parallel finished in " << duration.count() << " ms." << std::endl;

  // file PNG
  if (stbi_write_png("output/mandelbrot_parallel.png", WIDTH, HEIGHT, CHANNELS, image, WIDTH * CHANNELS)) {
    std::cout << "Image saved to mandelbrot_parallel.png" << std::endl;
  } else {
    std::cerr << "Error saving image." << std::endl;
  }

  delete[] image;
  return duration;
}

int main() {
  std::cout << "Choose the implementation (just number)" << '\n';
  std::cout << "1. Serial" << '\n';
  std::cout << "2. Parallel" << '\n';
  std::cout << "3. Parallel (CUDA)" << '\n';
  std::cout << "4. All (Compare speed)" << '\n';
  std::cout << ">> ";

  int opt = 0;
  std::cin >> opt;
  unsigned char* image = new unsigned char[WIDTH * HEIGHT * CHANNELS];

  if (opt == 1) {
    serial();
  } else if (opt == 2) {
    parallel();
  } else if (opt == 3) {
    auto dur_cuda = mandelbrot_cuda(image); // Sediakan buffer
    std::cout << "CUDA generation finished in " << dur_cuda.count() << " ms." << std::endl;
    stbi_write_png("output/mandelbrot_cuda.png", WIDTH, HEIGHT, CHANNELS, image, WIDTH * CHANNELS);
    std::cout << "Image saved to mandelbrot_cuda.png" << std::endl;
  } else if (opt == 4) {
    auto dur_serial = serial();
    auto dur_parallel = parallel();

    auto dur_cuda = mandelbrot_cuda(image);

    std::cout << "\n--- BENCHMARK RESULTS ---\n";
    std::cout << "Serial Time: " << dur_serial.count() << " ms\n";
    std::cout << "Parallel CPU Time: " << dur_parallel.count() << " ms\n";
    std::cout << "GPU CUDA Time: " << dur_cuda.count() << " ms\n";

    double speedup_cpu = (double)dur_serial.count() / dur_parallel.count();
    double speedup_gpu = (double)dur_serial.count() / dur_cuda.count();
    std::cout << "Speedup (Serial / Parallel CPU): " << speedup_cpu << "x\n";
    std::cout << "Speedup (Serial / GPU): " << speedup_gpu << "x\n";

    stbi_write_png("output/mandelbrot_cuda.png", WIDTH, HEIGHT, CHANNELS, image, WIDTH * CHANNELS);
  } else {
    std::cout << "Invalid option." << std::endl;
  }

  delete[] image;
  return 0;
}