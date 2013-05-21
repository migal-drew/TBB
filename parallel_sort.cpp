#include "tbb/parallel_sort.h"
#include <stdio.h>
#include <math.h>

using namespace tbb;

const int N = 20;
float a[N];
float b[N];

void SortExample()
{
    for( int i = 0; i < N; i++ )
    {
       a[i] = sin((double)i);
       printf("%3.3f ", a[i]);
    }
    printf("\n");
    parallel_sort(a, a + N);
    printf("Sorted:\n");
    for( int i = 0; i < N; i++ )
        printf("%3.3f ", a[i]);
    printf("\n");
}

int main()
{
    SortExample();
}
