#ifndef KMEANS_H
#define KMEANS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>
using namespace std;

#define maxV 0x3f3f3f3f
#define minV -maxV

#define vecPixel vector<Pixel*>
#define vecInt vector<int>
#define vecDouble vector<double>

#define max(a, b) ((a >= b) ? (a) : (b))


class Pixel
{
    public:
        Pixel(): B(0), G(0), R(0) {}

        double B;      //B通道
        double G;      //G通道
        double R;      //R通道
};

struct kMeansRes_
{
    vecPixel* centroids;           //主色
    vecInt* index;                 //归类
    double minDistSplit;           //主色距离
    double silhouCoeff;            //轮廓系数
};

inline Pixel* imageMin(vecPixel* image);
inline Pixel* imageMax(vecPixel* image);

Pixel* randCent(vecPixel* image);

double distance(Pixel* pixel, Pixel* image);

inline double Sum(vecDouble* vec);

kMeansRes_* kMeans(vecPixel* image, int k);

vecPixel* binaryKmeans(vecPixel* image, int k);
#endif
