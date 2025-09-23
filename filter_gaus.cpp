#include "bmp_reader.h"
#include <cmath>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

// Its function for generate the kernel for Gaussian filter
void GenereteGausKernel(int size, double sigma, double** gaussianKernel){
    double sum = 0.0;
    const double PI = 3.14159265;
    
    //double gaussianKernel[length_gaus][height_gaus];
    for (int height = 0; height < size; ++height) {
        for (int length = 0; length < size; ++length) {
            gaussianKernel[height][length] = (1 / (2 * PI * sigma * sigma)) *
                (exp(-(height * height + length * length)) / (2 * sigma * sigma)) ; 
            
            sum += gaussianKernel[height][length]; // For stabilization Gaus karnel
        }
    }
    
    // Stabilization Gaus karnel
    for (int height = 0; height < size; height++) {
        for (int length = 0; length < size; length++) {
            gaussianKernel[height][length] /= sum; 
        }
    }
}

// Function to apply Gaussian filter to an image
void ApplyGaussianFilter(BMP_File* bmp_file, int size, double sigma, int thread_count) {
    int width = bmp_file->dib_header.width;
    int height = bmp_file->dib_header.height;

    RGB* file_data = new RGB[width * height];

    double** gaussianKernel = new double*[size];
    for (int i = 0; i < size; ++i) gaussianKernel[i] = new double[size];
    GenereteGausKernel(size, sigma, gaussianKernel);

    auto worker = [&](int y_start, int y_end) {
        for (int y = y_start; y < y_end; ++y) {
            if (y == 0 || y == height - 1) continue;
            for (int x = 1; x < width - 1; ++x) {
                double red = 0.0, green = 0.0, blue = 0.0;

                for (int ky = -1; ky <= 1; ++ky) {
                    for (int kx = -1; kx <= 1; ++kx) {
                        RGB pixel = bmp_file->file_data[(y + ky) * width + (x + kx)];
                        double weight = gaussianKernel[ky + 1][kx + 1];
                        blue += pixel.blue * weight;
                        green += pixel.green * weight;
                        red += pixel.red * weight;
                    }
                }

                file_data[y * width + x] = RGB(
                    static_cast<uint8_t>(std::min(std::max(red, 0.0), 255.0)),
                    static_cast<uint8_t>(std::min(std::max(green, 0.0), 255.0)),
                    static_cast<uint8_t>(std::min(std::max(blue, 0.0), 255.0))
                );
            }
        }
    };

    std::vector<std::thread> threads;
    int block = height / thread_count;
    for (int i = 0; i < thread_count; ++i) {
        int y_start = i * block;
        int y_end = (i == thread_count - 1) ? height : y_start + block;
        threads.emplace_back(worker, y_start, y_end);
    }

    for (auto& t : threads) t.join();

    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            bmp_file->file_data[y * width + x] = file_data[y * width + x];

    delete[] file_data;
    for (int i = 0; i < size; ++i) delete[] gaussianKernel[i];
    delete[] gaussianKernel;
}
