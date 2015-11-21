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
        Pixel(): B(0), G(0), R(0), totalSSE(0) {}

        double B;
        double G;
        double R;
        double totalSSE;
};


class subPixel_
{
    public:
        subPixel_(): index(-1), SSE(0) {}

        int index;
        double SSE;
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


Pixel* randCent(vecPixel* image, int k)
{
    Pixel* centroids = new Pixel[k];

    Pixel* minPixel = imageMin(image);
    Pixel* maxPixel = imageMax(image);

    srand((unsigned)time(0));

    for(int i = 0; i < k; ++i)
    {
        centroids[i].B = minPixel->B + (maxPixel->B - minPixel->B) * (rand() / (double)(RAND_MAX));
        centroids[i].G = minPixel->G + (maxPixel->G - minPixel->G) * (rand() / (double)(RAND_MAX));
        centroids[i].R = minPixel->R + (maxPixel->R - minPixel->R) * (rand() / (double)(RAND_MAX));
    }

    return centroids;
}


inline double distance(Pixel& pixel, Pixel* image)
{
    return sqrt(pow(pixel.B - image->B, 2) + pow(pixel.G - image->G, 2) + pow(pixel.R - image->R, 2));
}


Pixel* kmeans(vecPixel* image, int k)
{
    const int len       = image->size();
    Pixel* centroids    = randCent(image, k);
    subPixel_* subPixel = new subPixel_[len];
    bool clusterChanged = true;

    while(clusterChanged)
    {
        clusterChanged = false;

        for(int i = 0; i < len; ++i)
        {
            int index   = -1;
            double dist = maxValue;

            for(int index_ = 0; index_ < k; ++index_)
            {
                double dist_ = distance(centroids[index_], image->at(i));
                if(dist_ < dist)   
                {
                    dist = dist_;
                    index = index_;
                }
            }

            if(subPixel[i].index != index)
                clusterChanged = true;

            subPixel[i].index = index;
            subPixel[i].SSE   = dist;
        }
    }

    int* cnt = new int[k];
    for(int i = 0; i < k; ++i)
        cnt[i] = 0;

    for(int i = 0; i < len; ++i)
    {
        cnt[subPixel[i].index]                += 1;
        centroids[subPixel[i].index].B        += image->at(i)->B;
        centroids[subPixel[i].index].G        += image->at(i)->G;
        centroids[subPixel[i].index].R        += image->at(i)->R;
        centroids[subPixel[i].index].totalSSE += subPixel[i].SSE;
    }

    for(int i = 0; i < k; ++i)
    {
        centroids[i].B /= cnt[i];
        centroids[i].G /= cnt[i];
        centroids[i].R /= cnt[i];
    }

    delete[] cnt;

    return centroids;
}
