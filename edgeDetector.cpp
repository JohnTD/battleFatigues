#include "edgeDetector.h"


void matrixRotate(double* src, double* dst, int height, int width)
{
    assert(src != NULL && dst != NULL && height > 0 && width > 0);

    double* temp = new double[height*width];

    //for(int i = 0; i < height; ++i)
    //{
        //for(int j = 0; j < width; ++j)
            //temp[i*width + j] = src[(height-i)*width + (width-j)];
    //}
    for(int i=0;i<width*height;i++){
        temp[width*height-1-i]=src[i];
    }

    for(int i = 0; i < height*width; ++i)
        dst[i] = temp[i];

    delete[] temp;
}


void Relevant(double* src, double* dst, double* mask, int height, int width, int mask_height, int mask_width)
{
    assert(src != NULL && dst != NULL && height > 0 && width > 0 && mask_height > 0 && mask_width > 0 && mask_height % 2 != 0 && mask_width % 2 != 0);   

    double* temp = new double[height*width];
    
    int mask_x = mask_height/2;
    int mask_y = mask_width/2;

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            double value = 0;
            for(int m = 0; m < mask_height; ++m)
            {
                for(int n = 0; n < mask_width; ++n)
                {
                    if((i+m-mask_x) >= 0 && (i+m-mask_x) < height && (j+n-mask_y) >= 0 && (j+n-mask_y) < width)
                        value += src[((i+m-mask_x)*width + j+n-mask_y)]*mask[m*mask_width + n];
                }
            }
            temp[i*width+j] = value;
        }
    }

    for(int i = 0; i < height*width; ++i)
        dst[i] = temp[i];

    delete[] temp;
}


void Convolution(double* src, double* dst, double* mask, int height, int width, int mask_height, int mask_width)
{
    assert(src != NULL && dst != NULL && height > 0 && width > 0 && mask_height > 0 && mask_width > 0);   

    double* temp = new double[height*width];

    matrixRotate(mask, temp, mask_height, mask_width);

    Relevant(src, dst, temp, height, width, mask_height, mask_width);

    delete[] temp;
}


void matrixNormalized(double* matrix, double value, int height, int width)
{
    for(int i = 0; i < height*width; ++i)
        matrix[i] /= value;
}


void getEdgeAngle(double* edge_x, double* edge_y, double* edgeAngle, int height, int width)
{
    assert(edge_x != NULL && edge_y != NULL && edgeAngle != NULL && height > 0 && width > 0);

    double x, y, value;

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            if((x != 0) && (y != 0))
            {
                x = edge_x[i*width+j];
                y = edge_y[i*width+j];
                value = atan2(y, x)*180/PI;
                if(value < 0)
                    value += 360;
                edgeAngle[i*width+j] = value;
            }
            else
                edgeAngle[i*width+j] = -1;
        }
    }
}


void Sobel(double* src, double* dst, double* edgeDirection, int height, int width)
{
    assert(src != NULL && dst != NULL && edgeDirection != NULL && height > 0 && width > 0);   

    double* dst_x = new double[height*width];
    double* dst_y = new double[height*width];

    double sobelMask1[3] = {1, 0, -1};
    double sobelMask2[3] = {0.25, 0.5, 0.25};

    Convolution(src, dst_x, sobelMask1, height, width, 1, 3);
    Convolution(dst_x, dst_x, sobelMask2, height, width, 3, 1);

    Convolution(src, dst_y, sobelMask2, height, width, 1, 3);
    Convolution(dst_y, dst_y, sobelMask1, height, width, 3, 1);

    if(!edgeDirection)
        getEdgeAngle(dst_x, dst_y, edgeDirection, height, width);

    for(int i = 0; i < height*width; ++i)
        dst[i] = fabs(dst_x[i]) + fabs(dst_y[i]);

    delete[] dst_x;
    delete[] dst_y;
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
    Convolution(src, temp, gaussianMask, height, width, 5, 5);
    matrixNormalized(temp, 159, height, width);

    //sobel边缘检测
    Sobel(temp, edge_A, edge_D, height, width);

    for(int i = 0; i < height*width; ++i)
        dst[i] = edge_A[i];
}
