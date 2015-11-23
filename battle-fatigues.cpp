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

    kMeansRes_* kMeansRes = new kMeansRes_;
    kMeansRes = kMeans(image, 4);
    for(int i = 0; i < kMeansRes->centroids->size(); ++i)
        cout<<kMeansRes->centroids->at(i)->B<<" "<<kMeansRes->centroids->at(i)->B<<" "<<kMeansRes->centroids->at(i)->B<<endl;

    cout<<"-----------"<<endl;

//    vecPixel* centroids = new vecPixel;
//    centroids = binaryKmeans(image, 4);
//
//    if(centroids->size() != 0)
//    {
//        for(int i = 0; i < centroids->size(); ++i)
//        {
//            cout<<centroids->at(i)->B<<" "<<centroids->at(i)->G<<" "<<centroids->at(i)->R<<endl;
//        }
//    }
}
