#include <stdio.h>
#include <stdlib.h>
#include "tbb/blocked_range.h"
#include "tbb/partitioner.h"
#include "tbb/parallel_for.h"
#include "tbb/parallel_reduce.h"
#include "tbb/task.h"
#include "tbb/tick_count.h"
#include <cfloat>
#include <time.h>
#include <fcntl.h>s

using namespace tbb;

long SerialMinIndex( const float a[], size_t n ) {
    float value_of_min = FLT_MAX;
    long index_of_min = -1;
    for( size_t i=0; i<n; ++i ) {
        float value = a[i];
        if( value<value_of_min ) {
            value_of_min = value;
            index_of_min = i;
        }
    }
    return index_of_min;
}

class MinIndexFoo {
    const float *const my_a;
public:
    float value_of_min;
    long index_of_min;
    void operator()( const blocked_range<size_t>& r ) {
        const float *a = my_a;
        for( size_t i=r.begin(); i!=r.end(); ++i ) {
           float value = a[i];
           if( value<value_of_min ) {
               value_of_min = value;
               index_of_min = i;
           }
        }
    }

    MinIndexFoo( MinIndexFoo& x, split ) :
        my_a(x.my_a),
        value_of_min(FLT_MAX),
        index_of_min(-1)
   {}

    void join( const MinIndexFoo& y ) {
        if( y.value_of_min<value_of_min ) {
            value_of_min = y.value_of_min;
            index_of_min = y.index_of_min;
        }
    }

    MinIndexFoo( const float a[] ) :
        my_a(a),
        value_of_min(FLT_MAX),
        index_of_min(-1)
    {}
};

void generate(float* arr, long n)
{
    float max = 10000;
    srand(time(NULL));
    for (long i = 0; i < n; i++)
        arr[i] = (float)rand() / ((float)RAND_MAX / max);

    /*
    parallel_for(blocked_range<size_t>(0, n),
                 [=](const blocked_range<size_t> &r)
                 {
                     for (size_t i = r.begin(); i != r.end(); i++)
                        arr[i] = (float)rand() / ((float)RAND_MAX / max);
                 }
    );
    */
}

int main()
{
    unsigned long n = 699000000;
    float * a = new float[n];
    //generate(a, n);
    int fd = open("/dev/random", O_RDONLY);
    read(fd, a, n);

    MinIndexFoo mif(a);

    tick_count st1 = tick_count::now();
    parallel_reduce(blocked_range<size_t>(0,n), mif );
    tick_count end1 = tick_count::now();
    printf("Parallel work took %g seconds\n",(end1 - st1).seconds());
    printf("Result is %f\n", mif.value_of_min);
    printf("Index is %d\n", mif.index_of_min);

    tick_count st2 = tick_count::now();
    long ind = SerialMinIndex(a, n);
    tick_count end2 = tick_count::now();
    printf("\n");
    printf("Sequential work took %g seconds\n",(end2 - st2).seconds());
    printf("Index of min %d\n", ind);
    delete[] a;
}
