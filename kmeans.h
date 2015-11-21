#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>
using namespace std;

#define minValue -0xffffffff 
#define maxValue 0xffffffff 

#define vecNode vector<Node*>

struct Node
{
    double x;
    double y;
    int cnt;
};

inline void arrayInit(int arr[], int len, int val)
{
    for(int i = 0; i < len; ++i)
        arr[i] = val;
}

inline void nodeInit(Node* node, int len)
{
    for(int i = 0; i < len; ++i)
    {
        node[i].x = 0;
        node[i].y = 0;
        node[i].cnt = 0;
    }
}

inline Node* arrayMin(vecNode* arr)
{
    Node* node = new Node;

    double minX = maxValue;
    double minY = maxValue;

    for(int i = 0; i < arr->size(); ++i)
    {
        if(arr->at(i)->x < minX)
            minX = arr->at(i)->x;
        if(arr->at(i)->y < minY)
            minY = arr->at(i)->y;
    }

    node->x = minX;
    node->y = minY;

    return node;
}

inline Node* arrayMax(vecNode* arr)
{
    Node* node = new Node;

    double maxX = minValue;
    double maxY = minValue;

    for(int i = 0; i < arr->size(); ++i)
    {
        if(arr->at(i)->x > maxX)
            maxX = arr->at(i)->x;
        if(arr->at(i)->y > maxY)
            maxY = arr->at(i)->y;
    }

    node->x = maxX;
    node->y = maxY;

    return node;
}

inline double distance(Node& node, Node* arr)
{
    return sqrt(pow(node.x - arr->x, 2) + pow(node.y - arr->y, 2));
}

Node* randCent(vecNode* arr, int k)
{
    Node* centroids = new Node[k];

    Node* minN = arrayMin(arr);
    Node* maxN = arrayMax(arr);

    srand((unsigned)time(0));

    for(int i = 0; i < k; ++i)
    {
        centroids[i].x = minN->x + (maxN->x - minN->x) * (rand() / (double)(RAND_MAX));
        centroids[i].y = minN->y + (maxN->y - minN->y) * (rand() / (double)(RAND_MAX));
    }

    return centroids;
}

void kmeans(vecNode* arr, int k)
{
    const int len = arr->size();

    int* index = new int[len];
    arrayInit(index, len, -1);
    int a = 0;

    Node* centroids = new Node[k];
    centroids = randCent(arr, k);

    bool clusterChanged = true;
    int index_ = -1;

    while(clusterChanged)
    {
        clusterChanged = false;

        for(int i = 0; i < len; ++i)
        {
            double dist = maxValue;

            for(int j = 0; j < k; ++j)
            {
                double dist_ = distance(centroids[j], arr->at(i));
                if(dist_ < dist)   
                {
                    dist = dist_;
                    index_ = j;
                }
            }

            if(index[i] != index_)
                clusterChanged = true;

            index[i] = index_;
        }
    }

    nodeInit(centroids, k);

    for(int i = 0; i < len; ++i)
    {
        centroids[index[i]].x += arr->at(i)->x;
        centroids[index[i]].y += arr->at(i)->y;
        centroids[index[i]].cnt += 1;
    }

    for(int i = 0; i < k; ++i)
    {
        if(centroids[i].cnt != 0)
        {
            centroids[i].x = centroids[i].x / centroids[i].cnt;
            centroids[i].y = centroids[i].y / centroids[i].cnt;
            cout<< "--- 第 " << i+1 << " 类 ---" <<endl
                << centroids[i].x << " " << centroids[i].y << endl;
        }
        else cout<< "第 " << i+1 << " 类总数为0" <<endl;
    }
    
    delete[] centroids;
    delete[] index;
}

void insertNode(vecNode* arr, double x, double y)
{
    Node* node = new Node;
    node->x = x;
    node->y = y;
    arr->push_back(node);
}

