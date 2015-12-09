#include "edgeDetector.h"

void matrixRotate(double* src, double* dst, int height, int width)
{
    assert(src != NULL && dst != NULL && height > 0 && width > 0);

    double* temp = new double[height*width];

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
            temp[i*width + j] = src[(height-i)*width + (width-j)];
    }

    for(int i = 0; i < height*width; ++i)
        dst[i] = temp[i];

    delete[] temp;
}

void Relevant(double* src, double* dst, double* mask, int height, int width, int mask_height, int mask_width)
{
    assert(src != NULL && dst != NULL && height > 0 && width > 0 && mask_height > 0 && mask_width > 0);   
    
    int mask_x = mask_height/2;
    int mask_y = mask_width/2;

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            double value = 0;
            for(int m = -mask_x; m <= mask_x; ++m)
            {
                for(int n = -mask_y; n <= mask_y; ++n)
                {
                    if(((i+m)*width) >= 0 && ((i+m)*width) < height && (j+n) >= 0 && (j+n) < width)
                        value += src[((i+m)*width + j+n)]*mask[(m+mask_x)*mask_width+n+mask_y];
                }
            }
            dst[i*width+j] = value;
        }
    }
}

void convolution(double* src, double* dst, double* mask, int height, int width, int mask_height, int mask_width)
{
    assert(src != NULL && dst != NULL && height > 0 && width > 0 && mask_height > 0 && mask_width > 0);   

    double* temp = new double[height*width];

    matrixRotate(mask, temp, mask_height, mask_width);

    Relevant(src, dst, temp, height, width, mask_height, mask_width);

    delete[] temp;
}

void Canny(double* src, double* dst, int threshold_h, int threshold_l, int height, int width)
{
    assert(src != NULL && dst != NULL && height > 0 && width > 0);   

    double* temp = new double[height*width];
    double* edge_A = new double[height*width];
    double* edge_D = new double[height*width];
    double* threshold_H = new double[height*width];
    double* threshold_L = new double[height*width];

    //高斯滤波
    double gaussianMask[25]={ 2, 4, 5, 4, 2,
                              4, 9,12, 9, 4,
                              5,12,15,12, 5,
                              4, 9,12, 9, 4,
                              2, 4, 5, 4, 2};
    convolution(src, temp, gaussianMask, height, width, 5, 5);
}
