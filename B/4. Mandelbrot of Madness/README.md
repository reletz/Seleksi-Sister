# Mandelbrot of Madness

## Overview

Mandelbrot of Madness :
- Serial (CPU single-thread)
- Parallel (CPU multi-thread/OpenMP)
- GPU (CUDA)

## Folder Structure

```
.
├── README.md
├── bin/
│   └── mandelbrot              # Binary
├── compile.sh                  # Script compile
├── output/
│   ├── mandelbrot_cuda.png     # Output CUDA
│   ├── mandelbrot_parallel.png # Output Parallel CPU
│   └── mandelbrot_serial.png   # Output Serial CPU
└── src/
    ├── main.cpp
    ├── mandelbrot.cpp
    ├── mandelbrot.h
    ├── mandelbrot_cuda.cu
    └── stb_image_write.h
```

## Compilation

Jalankan di root folder:

```bash
sh compile.sh
```

Binary akan berada di folder `bin/`.

## Usage

Jalankan program:

```bash
./bin/mandelbrot
```

Output gambar ada di folder `output/`.

## Output

- `output/mandelbrot_serial.png`    : Hasil versi serial
- `output/mandelbrot_parallel.png`  : Hasil versi parallel CPU
- `output/mandelbrot_cuda.png`      : Hasil versi CUDA

## Benchmark

| Version        | Time (s) |  Speedup  |
| -------------- | -------- | --------- |
| Serial         |  4537 ms |     -     |
| Parallel (CPU) |  4020 ms |  1.12861x |
| CUDA (GPU)     |   561 ms |  8.08734x |


## Credits

- [stb_image_write.h](https://github.com/nothings/stb) oleh Sean Barrett
- Gemini

## Screenshots