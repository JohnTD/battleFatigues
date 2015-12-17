#include "edgeDetector.h"


void matrixRotate(double* src, double* dst, int height, int width)
{
    assert(src != NULL && dst != NULL && height > 0 && width > 0);

    double* temp = new double[height*width];

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
            temp[(height-1-i)*width + width-1-j] = src[i*width+j];
    }

    for(int i = 0; i < height*width; ++i)
        dst[i] = temp[i];

    delete[] temp;
}


double matrixMaxValue(double* matrix, int height, int width)
{
    double value = -1;

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; ++j)
        {
            if(matrix[i*width+j] > value)
                value = matrix[i*width+j];
        }
    }

    return value;
}


void Relevant(double* src, double* dst, double* mask, int height, int width, int mask_height, int mask_width)
{
    assert(src != NULL && dst != NULL && height > 0 && width > 0 && mask_height > 0 && mask_width > 0 && mask_height % 2 != 0 && mask_width % 2 != 0);   

    double* temp = new double[height*width];
    
    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            double value = 0;
            for(int m = 0; m < mask_height; ++m)
            {
                for(int n = 0; n < mask_width; ++n)
                {
                    if((i-mask_height+1 + m) >= 0 && (i-mask_height+1 + m) < height && (j-mask_width+1 + n) >= 0 && (j-mask_width+1 + n) < width)
                        value += src[((i-mask_height+1 + m)*width + j-mask_width+1 + n)]*mask[m*mask_width + n];
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
            x = edge_x[i*width+j];
            y = edge_y[i*width+j];
            if((x != 0) && (y != 0))
            {
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


/*
 *边缘方向
 *0  1  2
 *3  *  5
 *6  7  8
 */
void getEdgeDirection(double* edgeAngle, double* edgeDirection, int height, int width)
{
    assert(edgeAngle != NULL && edgeDirection != NULL && height > 0 && width > 0);

    double* temp = new double[height*width];

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            double angle = edgeAngle[i*width+j];
            if(angle >= 0 && angle < 45)
                temp[i*width+j] = 7;
            else if(angle >= 45 && angle < 90)
                temp[i*width+j] = 8;
            else if(angle >= 90 && angle < 135)
                temp[i*width+j] = 5;
            else if(angle >= 135 && angle < 180)
                temp[i*width+j] = 2;
            else if(angle >= 180 && angle < 225)
                temp[i*width+j] = 1;
            else if(angle >= 225 && angle < 270)
                temp[i*width+j] = 0;
            else if(angle >= 270 && angle < 315)
                temp[i*width+j] = 3;
            else if(angle >= 315 && angle < 360)
                temp[i*width+j] = 6;
            else
                temp[i*width+j] = -1;
        }
    }
    
    for(int i = 0; i < height*width; ++i)
        edgeDirection[i] = temp[i];

    delete[] temp;
}


void nonMaximumSuppression(double* edgeAmplitude, double* edgeDirection, int height, int width)
{
    assert(edgeAmplitude != NULL && edgeDirection != NULL && height > 0 && width > 0);

    double* temp = new double[height*width];

    for(int i = 1; i < height-1; ++i)
    {
        for(int j = 1; j < width-1; ++j)
        {
            if(edgeDirection[i*width+j] != -1)
            {
                int x = (int)edgeDirection[i*width+j] / 3 -1;
                int y = (int)edgeDirection[i*width+j] % 3 -1;
                if(edgeAmplitude[i*width+j] < edgeAmplitude[(i+x)*width+j+y] || edgeAmplitude[i*width+j] < edgeAmplitude[(i-x)*width+j-y])
                {
                    temp[i*width+j] = 0;
                }
                else
                    temp[i*width+j] = edgeAmplitude[i*width+j];
            }
            else
                temp[i*width+j] = 0;
        }
    }

    for(int i = 0; i < height*width; ++i)
        edgeAmplitude[i] = temp[i];

    delete[] temp;
}


void edgeTrack(double* edgeAmplitude, Position* seed, int height, int width)
{
    assert(edgeAmplitude != NULL && seed != NULL && height > 0 && width > 0);

    int x = seed->x;
    int y = seed->y;

    if(x >= 0 && x < height && y >= 0 && y < width && edgeAmplitude[x*width+y] == 1.0)
    {
        edgeAmplitude[x*width+y] = 2.0;

        for(int i = -1; i < 2; ++i)
        {
            for(int j = -1; j < 2; ++j)
            {
                if((x != 0) && (y != 0))
                {
                    Position seedNext;
                    seedNext.x = x+i;
                    seedNext.y = y+j;
                    edgeTrack(edgeAmplitude, &seedNext, height, width);
                }
            }
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

    if(edgeDirection != NULL)
        getEdgeAngle(dst_x, dst_y, edgeDirection, height, width);

    for(int i = 0; i < height*width; ++i)
        dst[i] = fabs(dst_x[i]) + fabs(dst_y[i]);

    delete[] dst_x;
    delete[] dst_y;
}


void Canny(double* src, double* dst, int height, int width)
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

    //非极大值抑制
    getEdgeDirection(edge_D, edge_D, height, width);    
    nonMaximumSuppression(edge_A, edge_D, height, width);

    double value = matrixMaxValue(edge_A, height, width);
    double threshold_h = 0.01 * value;
    double threshold_l = 0.005 * value;

    //双阈值滞后滤波
    for(int i = 0; i < height*width; ++i)
    {
        threshold_H[i] = edge_A[i] > threshold_h ? 1.0 : 0.0;
    }
    for(int i = 0; i < height*width; ++i)
        threshold_L[i] = edge_A[i] > threshold_l ? 1.0 : 0.0;

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            if(threshold_H[i*width+j] == 1.0 && threshold_L[i*width+j] != 2.0)
            {
                Position p;
                p.x = i;
                p.y = j;
                edgeTrack(threshold_L, &p, height, width);
            }
        }
    }

    for(int i = 0; i < height*width; ++i)
        dst[i] = 0;
    for(int i = 0; i < height*width; ++i)
    {
        if(threshold_L[i] == 2.0)
        {
            dst[i] = 1.0;
        }
    }
}
