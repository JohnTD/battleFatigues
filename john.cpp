#include <vector>
#include "kmeans.h"

int main(int argc, char** argv)
{
    vecNode* arr = new vecNode[21];
    kmeans(arr, 3);
    delete[] arr;
}
