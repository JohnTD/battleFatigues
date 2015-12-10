#ifndef EDGEDETERCTOR_H
#define EDGEDETERCTOR_H

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

using namespace std;

#define PI 3.1415926535898

struct Position
{
    int x;
    int y;
};

void matrixRotate(double* src, double* dst, int height, int width);

double matrixMaxValue(double* matrix, int height, int width);

void Relevant(double* src, double* dst, double* mask, int height, int width, int mask_height, int mask_width);

void Convolution(double* src, double* dst, double* mask, int height, int width, int mask_height, int mask_width);

void matrixNormalized(double* src, double value, int height, int width);    

void getEdgeAngle(double* edge_x, double* edge_y, double* edgeAngle, int height, int width);

void getEdgeDirection(double* edgeAngle, double* edgeDirection, int height, int width);

void nonMaximumSuppression(double* edgeAmplitude, double* edgeDirection, int height, int width);

void edgeTrack(double* edgeAmplitude, Position* seed, int height, int width);

void Sobel(double* src, double* dst, double* edgeDirection, int height, int width);

void Canny(double* src, double* dst, int height, int width);

#endif
