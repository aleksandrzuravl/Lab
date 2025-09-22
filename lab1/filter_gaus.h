#ifndef FILTER_GAUS_H
#define FILTER_GAUS_H

#include "bmp_reader.h"

void ApplyGaussianFilter(BMP_File* bmp_file, int size, double sigma, int thread_count=4);

#endif
