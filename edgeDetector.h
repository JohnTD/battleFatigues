#ifndef EDGEDETERCTOR_H
#define EDGEDETERCTOR_H

#include <math.h>
#include <assert.h>

#define PI 3.1415926535898

void Convolution(double* src, double* dst, double* mask, int height, int width, int mask_height, int mask_width)
void matrixRotate(double* src, double* dst, int height, int width);
void Canny(double* src, double* dst, int threshold1, int threshold2, int height, int width);

#endif
