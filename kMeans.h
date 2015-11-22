#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>
using namespace std;

#define minValue -0xffffffff 
#define maxValue 0xffffffff 
#define vecPixel vector<Pixel*>
#define vecInt vector<int>
#define vecDouble vector<double>


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
            centroids = new vecPixel[centCnt];
            index     = new vecInt[pixelCnt];
            minDist   = new vecDouble[pixelCnt];
        }

        vecPixel* centroids;      //主色
        vecInt* index;            //归类
        vecDouble*  minDist;      //主色距离
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


void randCent(vecPixel* image, vecPixel* centroids, int k)
{
    if(image == NULL || centroids == NULL || k <= 0) return;

    Pixel* minPixel = imageMin(image);
    Pixel* maxPixel = imageMax(image);

    srand((unsigned)time(0));

    for(int i = 0; i < k; ++i)
    {
        centroids->at(i)->B = minPixel->B + (maxPixel->B - minPixel->B) * (rand() / (double)(RAND_MAX));
        centroids->at(i)->G = minPixel->G + (maxPixel->G - minPixel->G) * (rand() / (double)(RAND_MAX));
        centroids->at(i)->R = minPixel->R + (maxPixel->R - minPixel->R) * (rand() / (double)(RAND_MAX));
    }
}


inline double distance(Pixel* pixel, Pixel* image)
{
    return sqrt(pow(pixel->B - image->B, 2) + pow(pixel->G - image->G, 2) + pow(pixel->R - image->R, 2));
}


inline double sseSum(vecDouble* SSE)
{
    double sseSum_ = 0;
    for(int i = 0; i < SSE->size(); ++i)
    {
        sseSum_ += SSE->at(i);
    }
    return sseSum_;
}


kMeansRes_* kMeans(vecPixel* image, int k)
{
    const int len       = image->size();

    vecPixel* centroids = new vecPixel[k];
    vecInt* index       = new vecInt[len];
    vecDouble* minDist  = new vecDouble[len];

    randCent(image, centroids, k);

    bool clusterChanged = true;

    while(clusterChanged)
    {
        clusterChanged = false;

        for(int i = 0; i < len; ++i)
        {
            int index_      = -1;
            double minDist_ = maxValue;

            for(int j = 0; j < k; ++j)
            {
                double dist = distance(centroids->at(i), image->at(i));
                if(dist < minDist_)   
                {
                    minDist_  = dist;
                    index_    = j;
                }
            }

            if(index->at(i) != index_)
                clusterChanged = true;

            index->at(i)   = index_;
            minDist->at(i) = minDist_;
        }
    }

    int* cnt = new int[len];
    for(int i = 0; i < len; ++i)
        cnt[i] = 0;

    for(int i = 0; i < len; ++i)
    {
        cnt[index->at(i)]++;
        centroids->at(index->at(i))->B += image->at(i)->B;
        centroids->at(index->at(i))->G += image->at(i)->G;
        centroids->at(index->at(i))->R += image->at(i)->R;
    }

    for(int i = 0; i < k; ++i)
    {
        centroids->at(i)->B /= cnt[i];
        centroids->at(i)->G /= cnt[i];
        centroids->at(i)->R /= cnt[i];
    }

    delete[] cnt;

    kMeansRes_* kMeansRes = new kMeansRes_(len, k);
    kMeansRes->centroids  = centroids;
    kMeansRes->index      = index;
    kMeansRes->minDist    = minDist;

    return kMeansRes;
}


vecPixel* binaryKmeans(vecPixel* image, int k)
{
    const int len = image->size();

    vecPixel* centroids = new vecPixel;
    Pixel*    centPixel = new Pixel;
    vecInt*       index = new vecInt[len];
    vecDouble*      SSE = new vecDouble[len];

    for(int i = 0; i < len; ++i)
    {
        centPixel->B += image->at(i)->B/len;
        centPixel->G += image->at(i)->G/len;
        centPixel->R += image->at(i)->R/len;
    }
    centroids->push_back(centPixel);

    for(int i = 0; i < len; ++i)
        SSE->at(i) = distance(centPixel, image->at(i));

    while(centroids->size() < k)
    {
        double minSSE = maxValue;
        int bestCentToSplit;
        vecPixel* bestNewCent;
        vecInt* bestClustAss;
        for(int i = 0; i < centroids->size(); ++i)
        {
            vecPixel*      imageSplit = new vecPixel;
            vecPixel*   imageNotSplit = new vecPixel;
            vecDouble* minDistNoSplit = new vecDouble;

            for(int j = 0; j < len; ++j)
            {
                Pixel* pixel = new Pixel;
                pixel->B = image->at(j)->B;
                pixel->G = image->at(j)->G;
                pixel->R = image->at(j)->R;
                if(index->at(j) == i)
                    imageSplit->push_back(pixel);
                else
                {
                    imageNotSplit->push_back(pixel);
                    minDistNoSplit->push_back(SSE->at(j));
                }
            }
            kMeansRes_* kMeansRes = kMeans(imageSplit, 2);
            double       sseSplit = sseSum(kMeansRes->minDist);
            double    sseNotSplit = sseSum(minDistNoSplit);
            if(sseSplit + sseNotSplit < minSSE)
            {
                bestCentToSplit = i;
                bestNewCent = kMeansRes->centroids;
                bestClustAss = kMeansRes->index;
                minSSE = sseSplit + sseNotSplit;
            }
        }
        for(int i = 0; i < bestClustAss->size(); ++i)
        {
            if(bestClustAss->at(i) == 0)
                index->at(bestCentToSplit) = bestCentToSplit;
            else index->at(bestCentToSplit) = centroids->size();
        }
        centroids->at(bestCentToSplit) = bestNewCent->at(0);
        centroids->push_back(bestNewCent->at(1));
    }

    return centroids;
}
