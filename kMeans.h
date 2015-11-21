#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>
using namespace std;

#define minValue -0xffffffff 
#define maxValue 0xffffffff 

#define vecPixel vector<Pixel*>

struct Pixel
{
    double B;
    double G;
    double R;
    double SSE;
};

inline void imageInit(int image[], int len, int val)
{
    for(int i = 0; i < len; ++i)
        image[i] = val;
}

inline void pixelInit(Pixel* pixel, int len)
{
    for(int i = 0; i < len; ++i)
    {
        pixel[i].B = 0;
        pixel[i].G = 0;
        pixel[i].R = 0;
        pixel[i].SSE = 0;
    }
}

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
    pixel->SSE = 0;

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
    pixel->SSE = 0;

    return pixel;
}

inline double distance(Pixel& pixel, Pixel* image)
{
    return sqrt(pow(pixel.B - image->B, 2) + pow(pixel.G - image->G, 2) + pow(pixel.R - image->R, 2));
}

Pixel* randCent(vecPixel* image, int k)
{
    Pixel* centroids = new Pixel[k];

    Pixel* minN = imageMin(image);
    Pixel* maxN = imageMax(image);

    srand((unsigned)time(0));

    for(int i = 0; i < k; ++i)
    {
        centroids[i].x = minN->x + (maxN->x - minN->x) * (rand() / (double)(RAND_MAX));
        centroids[i].y = minN->y + (maxN->y - minN->y) * (rand() / (double)(RAND_MAX));
    }

    return centroids;
}

void kmeans(vecPixel* image, int k)
{
    const int len = image->size();

    Pixel* centroids = new Pixel[k];
    centroids = randCent(image, k);

    Pixel* clusterAss = new Pixel[len];
    pixelInit(clusterAss, len);

    bool clusterChanged = true;

    while(clusterChanged)
    {
        clusterChanged = false;

        for(int i = 0; i < len; ++i)
        {
            int index = -1;
            double dist = maxValue;

            for(int index_ = 0; j < k; ++j)
            {
                double dist_ = distance(centroids[j], image->at(i));
                if(dist_ < dist)   
                {
                    dist = dist_;
                    index = index_;
                }
            }

            if(clusterAss[i].x != index)
                clusterChanged = true;

            clusterAss[i].x = index;
            clusterAss[i].y = dist;
        }
    }

    pixelInit(centroids, k);

    int* cnt = new int[k];
    imageInit(cnt, k, -1);

    for(int i = 0; i < len; ++i)
    {
        centroids[index[i]].x += image->at(i)->x;
        centroids[index[i]].y += image->at(i)->y;
        cnt[index[i]] += 1;
    }

    for(int i = 0; i < k; ++i)
    {
        if(centroids[i].cnt != 0)
        {
            centroids[i].x = centroids[i].x / cnt[i];
            centroids[i].y = centroids[i].y / cnt[i];
            cout<< "--- 第 " << i+1 << " 类 ---" <<endl
                << centroids[i].x << " " << centroids[i].y << endl;
        }
        else cout<< "第 " << i+1 << " 类总数为0" <<endl;
    }
    
    delete[] centroids;
    delete[] index;
}

void insertPixel(vecPixel* image, double x, double y)
{
    Pixel* pixel = new Pixel;
    pixel->x = x;
    pixel->y = y;
    image->push_back(pixel);
}

