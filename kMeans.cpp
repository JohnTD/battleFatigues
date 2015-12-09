#include "kMeans.h"


inline Pixel* imageMin(vecPixel* image)
{
    Pixel* pixel = new Pixel;

    double minB = maxV;
    double minG = maxV;
    double minR = maxV;

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

    double maxB = minV;
    double maxG = minV;
    double maxR = minV;

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


Pixel* randCent(vecPixel* image)
{
    if(image->size() <= 0) return NULL;

    Pixel* minPixel = imageMin(image);
    Pixel* maxPixel = imageMax(image);

    srand((unsigned)time(0));

    Pixel* pixel = new Pixel;
    pixel->B     = minPixel->B + (maxPixel->B - minPixel->B) * (rand() / (double)(RAND_MAX));
    pixel->G     = minPixel->G + (maxPixel->G - minPixel->G) * (rand() / (double)(RAND_MAX));
    pixel->R     = minPixel->R + (maxPixel->R - minPixel->R) * (rand() / (double)(RAND_MAX));
    return pixel;
}


double distance(Pixel* pixel, Pixel* image)
{
    return sqrt(pow(pixel->B - image->B, 2) + pow(pixel->G - image->G, 2) + pow(pixel->R - image->R, 2));
}


inline double Sum(vecDouble* vec)
{
    double Sum_ = 0;
    for(int i = 0; i < vec->size(); ++i)
    {
        Sum_ += vec->at(i);
    }
    return Sum_;
}


kMeansRes_* kMeans(vecPixel* image, int k)
{
    if(image->size() == 0 || k < 1) return NULL;

    const int len = image->size();

    vecPixel* centroids     = new vecPixel;
    vecInt* index           = new vecInt(len, -1);
    vecDouble* minDistSplit = new vecDouble(len, 0);
    vecDouble* silhouCoeff    = new vecDouble(len, 0);

    //随机生成一个controid
    centroids->push_back(randCent(image));

    //kmeans++算法取初始controid
    for(int i = 0; i < k-1; ++i)
    {
        Pixel*      nextCent = new Pixel;
        double maxDistToCent = minV;
        for(int m = 0; m < len; ++m)
        {
            double pixelToCent = 0;
            for(int n = 0; n < centroids->size(); ++n)
            {
                pixelToCent += distance(centroids->at(n), image->at(m));
            }
            if(pixelToCent > maxDistToCent)
                nextCent = image->at(i);
        }
        centroids->push_back(nextCent);
    }

    bool clusterChanged = true;

    //聚类过程
    while(clusterChanged)
    {
        clusterChanged = false;

        //遍历image
        for(int i = 0; i < len; ++i)
        {
            int index_      = -1;
            double minDist_ = maxV;

            //比较第i点和各centroid距离选择最合适centroid
            for(int j = 0; j < k; ++j)
            {
                double dist = distance(centroids->at(j), image->at(i));
                if(dist < minDist_)   
                {
                    minDist_ = dist;
                    index_   = j;
                }
            }

            //第i点的归类情况发生变化
            if(index->at(i) != index_)
                clusterChanged = true;

            //更新归类数组和主色距离数组
            index->at(i)        = index_;
            minDistSplit->at(i) = minDist_;
        }

        int* cnt = new int[k];
        for(int i = 0; i < k; ++i)
            cnt[i] = 1;

        //统计各类别点数以及centroid赋值
        for(int i = 0; i < len; ++i)
        {
            cnt[index->at(i)]++;
            centroids->at(index->at(i))->B += image->at(i)->B;
            centroids->at(index->at(i))->G += image->at(i)->G;
            centroids->at(index->at(i))->R += image->at(i)->R;
        }

        //centroid求平均值
        for(int i = 0; i < k; ++i)
        {
            centroids->at(i)->B /= cnt[i];
            centroids->at(i)->G /= cnt[i];
            centroids->at(i)->R /= cnt[i];
        }
        
        delete[] cnt;
    }

    //计算K的轮廓系数Silhouette Coefficient
    for(int i = 0; i < len; ++i)
    {
        double cohesion = minDistSplit->at(i);
        double minSeparation = maxV;
        //由于各点到其他所有点的计算量较大，改为各点到centroid
        for(int j = 0; j < k; ++j)
        {
            if(index->at(i) != j)
            {
                double scDist = distance(image->at(i), centroids->at(j));
                if(scDist < minSeparation)
                    minSeparation = scDist;
            }
        }
        silhouCoeff->at(i) = (minSeparation - cohesion) / max(cohesion, minSeparation);
    }

    kMeansRes_* kMeansRes   = new kMeansRes_;
    kMeansRes->centroids    = centroids;
    kMeansRes->index        = index;
    kMeansRes->minDistSplit = Sum(minDistSplit);
    kMeansRes->silhouCoeff  = Sum(silhouCoeff)/(silhouCoeff->size());

    return kMeansRes;
}


vecPixel* binaryKmeans(vecPixel* image, int k)
{
    if(image->size() == 0 || k < 1) return NULL;

    const int len = image->size();

    vecPixel* centroids = new vecPixel;
    Pixel*    centPixel = new Pixel;
    vecInt*       index = new vecInt(len, 0);
    vecDouble*      SSE = new vecDouble(len, 0);

    for(int i = 0; i < len; ++i)
    {
        centPixel->B += image->at(i)->B/len;
        centPixel->G += image->at(i)->G/len;
        centPixel->R += image->at(i)->R/len;
    }
    centroids->push_back(centPixel);

    for(int i = 0; i < len; ++i)
    {
        SSE->at(i) = distance(centPixel, image->at(i));
    }

    while(centroids->size() < k)
    {
        double         minSSE = maxV;
        int   bestCentToSplit = -1;
        vecPixel* bestNewCent = NULL;
        vecInt*  bestClustAss = NULL;
        for(int i = 0; i < centroids->size(); ++i)
        {
            vecPixel*      imageSplit = new vecPixel;
            vecPixel*   imageNotSplit = new vecPixel;
            vecDouble* minDistNoSplit = new vecDouble;

            for(int j = 0; j < len; ++j)
            {
                Pixel* pixel = new Pixel;
                pixel->B     = image->at(j)->B;
                pixel->G     = image->at(j)->G;
                pixel->R     = image->at(j)->R;
                if(index->at(j) == i)
                    imageSplit->push_back(pixel);
                else
                {
                    imageNotSplit->push_back(pixel);
                    minDistNoSplit->push_back(SSE->at(j));
                }
            }
            kMeansRes_* kMeansRes = kMeans(imageSplit, 2);
            if(kMeansRes != NULL)
            {
                double    sseSplit = kMeansRes->minDistSplit;
                double sseNotSplit = Sum(minDistNoSplit);
                if(sseSplit + sseNotSplit < minSSE)
                {
                    bestCentToSplit = i;
                    bestNewCent     = kMeansRes->centroids;
                    bestClustAss    = kMeansRes->index;
                    minSSE          = sseSplit + sseNotSplit;
                }
            }
        }

        if(bestNewCent != NULL && bestClustAss != NULL)
        {
            for(int i = 0; i < bestClustAss->size(); ++i)
            {
                if(bestClustAss->at(i) == 0)
                    index->at(bestCentToSplit)  = bestCentToSplit;
                else index->at(bestCentToSplit) = centroids->size();
            }
            centroids->at(bestCentToSplit) = bestNewCent->at(0);
            centroids->push_back(bestNewCent->at(1));
        }
    }

    return centroids;
}
