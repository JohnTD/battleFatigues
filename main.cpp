#include "kMeans.h"
#include "edgeDetector.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

int main(int argc, char** argv)
{
    Mat img = imread(argv[1], 0);
    img.convertTo(img, CV_64FC1, 1.0/255.0);

    int height = img.rows;
    int width = img.cols;

/*
 *canny边缘检测
 */
    double* src = new double[height*width];
    double* dst = new double[height*width];
    double* edge = new double[height*width];

    for(int i = 0; i < height; ++i)
    {
        double* u = img.ptr<double>(i);
        for(int j = 0; j < width; ++j)
        {
            src[i*width+j] = *u++;
        }
    }

    Canny(src, dst, height, width);

    for(int i = 0; i < height; ++i)
    {
        double* u = img.ptr<double>(i);
        for(int j = 0; j < width; ++j)
        {
            *u++ = dst[i*width+j];
        }
    }

/*
 *k-means++聚类寻找图像主色
 */
    vecPixel* image = new vecPixel;

    for(int i = 0; i < height; ++i)
    {
        double* u = img.ptr<double>(i);
        for(int j = 0; j < width; ++j)
        {
            Pixel* pixel = new Pixel;
            pixel->B = *u++;
            pixel->G = *u++;
            pixel->R = *u++;
            image->push_back(pixel);
        }
    }

    kMeansRes_* kMeansRes;

    vecPixel* bestClustCent = new vecPixel;
    int bestClustIndex      = -1;

    double maxSC = minV;

    int begin = 2;
    int end   = 8;
    int times = 10;

    for(int i = begin; i < end; ++i)
    {
        double scMean = 0;
        for(int j = 0; j < times; ++j)
        {
            kMeansRes = kMeans(image, i);
            scMean   += kMeansRes->silhouCoeff / times;
        }
        if(scMean > maxSC)
        {
            maxSC = scMean;
            bestClustIndex = i;
        }
    }
    bestClustCent = kMeans(image, bestClustIndex)->centroids;
}
