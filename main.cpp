#include "kMeans.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

using namespace cv;

int main(int argc, char** argv)
{
    Mat img = imread(argv[1], IMREAD_COLOR);
    int nr = img.rows;
    int nc = img.cols;

    vecPixel* image = new vecPixel;

    for(int i = 0; i < nr; ++i)
    {
        for(int j = 0; j < nc; ++j)
        {
            Pixel* pixel = new Pixel;
            pixel->B = (double)img.at<Vec3b>(i, j)[0];
            pixel->G = (double)img.at<Vec3b>(i, j)[1];
            pixel->R = (double)img.at<Vec3b>(i, j)[2];
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

    for(int i = 0; i < bestClustCent->size(); ++i)
    {
        string s;
        char t[256];
        sprintf(t, "%d", i);
        s = t;
        Mat test(500, 500, CV_8UC3, Scalar((int)bestClustCent->at(i)->B, (int)bestClustCent->at(i)->G, (int)bestClustCent->at(i)->R));
        namedWindow(s, WINDOW_AUTOSIZE);
        imshow(s, test);
    }
    waitKey(0);
}
