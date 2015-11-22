#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>
using namespace std;

#define minValue -0xffffffff 
#define maxValue 0xffffffff 
#define vecPixel vector<Pixel*>


class Pixel
{
    public:
        Pixel(): B(0), G(0), R(0) {}

        double B;      //B通道
        double G;      //G通道
        double R;      //R通道
};

class kMeansRes_
{
    public:
        kMeansRes_(int centCnt, int pixelCnt)
        {
            centroids = new Pixel[centCnt];
            index     = new int[pixelCnt];
            SSE       = new double[pixelCnt];
        }

        Pixel* centroids;      //主色
        int* index;            //归类
        double* SSE;           //主色距离
};


inline Pixel* imageMin(vecPixel* image)
{
    Pixel* pixel = new Pixel;

    double minB = maxValue;
    double minG = maxValue;
    double minR = maxValue;

    for(int i = 0; i < image->size(); ++i)
    {
        if(image->at(i)->B < minB)
            minB = image->at(i)->B;
        if(image->at(i)->G < minG)
            minG = image->at(i)->G;
        if(image->at(i)->R < minR)
            minR = image->at(i)->R;
    }

    pixel->B = minB;
    pixel->G = minG;
    pixel->R = minR;

    return pixel;
}


inline Pixel* imageMax(vecPixel* image)
{
    Pixel* pixel = new Pixel;

    double maxB = minValue;
    double maxG = minValue;
    double maxR = minValue;

    for(int i = 0; i < image->size(); ++i)
    {
        if(image->at(i)->B > maxB)
            maxB = image->at(i)->B;
        if(image->at(i)->G > maxG)
            maxG = image->at(i)->G;
        if(image->at(i)->R > maxR)
            maxR = image->at(i)->R;
    }

    pixel->B = maxB;
    pixel->G = maxG;
    pixel->R = maxR;

    return pixel;
}


void randCent(vecPixel* image, Pixel* centroids, int k)
{
    if(image == NULL || centroids == NULL || k <= 0) return;

    Pixel* minPixel = imageMin(image);
    Pixel* maxPixel = imageMax(image);

    srand((unsigned)time(0));

    for(int i = 0; i < k; ++i)
    {
        centroids[i].B = minPixel->B + (maxPixel->B - minPixel->B) * (rand() / (double)(RAND_MAX));
        centroids[i].G = minPixel->G + (maxPixel->G - minPixel->G) * (rand() / (double)(RAND_MAX));
        centroids[i].R = minPixel->R + (maxPixel->R - minPixel->R) * (rand() / (double)(RAND_MAX));
    }
}


inline double distance(Pixel& pixel, Pixel* image)
{
    return sqrt(pow(pixel.B - image->B, 2) + pow(pixel.G - image->G, 2) + pow(pixel.R - image->R, 2));
}


kMeansRes_* kmeans(vecPixel* image, int k)
{
    const int len       = image->size();

    Pixel* centroids = new Pixel[k];
    int* index       = new int[len];
    double* SSE      = new double[len];

    randCent(image, centroids, k);

    bool clusterChanged = true;

    while(clusterChanged)
    {
        clusterChanged = false;

        for(int i = 0; i < len; ++i)
        {
            int index_     = -1;
            double minDist = maxValue;

            for(int j = 0; j < k; ++j)
            {
                double dist = distance(centroids[j], image->at(i));
                if(dist < minDist)   
                {
                    minDist  = dist;
                    index_ = j;
                }
            }

            if(index[i] != index_)
                clusterChanged = true;

            index[i] = index_;
            SSE[i]   = minDist;
        }
    }

    int* cnt = new int[len];
    for(int i = 0; i < len; ++i)
        cnt[i] = 0;

    for(int i = 0; i < len; ++i)
    {
        cnt[index[i]]++;
        centroids[index[i]].B += image->at(i)->B;
        centroids[index[i]].G += image->at(i)->G;
        centroids[index[i]].R += image->at(i)->R;
    }

    for(int i = 0; i < k; ++i)
    {
        centroids[i].B /= cnt[i];
        centroids[i].G /= cnt[i];
        centroids[i].R /= cnt[i];
    }

    delete[] cnt;

    kMeansRes_* kMeansRes = new kMeansRes_(len, k);
    kMeansRes->centroids  = centroids;
    kMeansRes->index   = index;
    kMeansRes->SSE   = SSE;

    return kMeansRes;
}

Pixel* binaryKmeans(vecPixel* image, int k)
{
        
}
