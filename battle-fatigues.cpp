#include <iostream>
#include <stdio.h>
#include <vector>
#include "kMeans.h"

int main(int argc, char** argv)
{
    freopen("./testSet.txt", "r", stdin);
    vecNode* arr = new vecNode;
    double x, y;
    while(cin>>x>>y)
        insertNode(arr, x, y);
    
    kmeans(arr, 4);
    delete[] arr;
}
