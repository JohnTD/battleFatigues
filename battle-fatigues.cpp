#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include "kMeans.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    vecPixel* image = new vecPixel;
    vecPixel* centroids = new vecPixel;
    Mat img = imread(argv[1], IMREAD_COLOR);
    int nr = img.rows;
    int nc = img.cols;
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
    centroids = binaryKmeans(image, 100);
    for(int i = 0; i < centroids->size(); ++i)
    {
        cout<<centroids->at(i)->B<<" "<<centroids->at(i)->G<<" "<<centroids->at(i)->R<<endl;
    }
}
