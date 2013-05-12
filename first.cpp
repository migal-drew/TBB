#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/task_scheduler_init.h"
#include <iostream>
#include <vector>

using namespace tbb;
using namespace std;

struct Average {
    const float* input;
    float* output;
    void operator()( const blocked_range<int>& range ) const {
        for( int i=range.begin(); i!=range.end(); ++i ) {
            output[i] = (input[i-1]+input[i]+input[i+1])*(1/3.f);
            cout << output[i] << ' ';
        }
    }
};

// Note: Reads input[0..n] and writes output[1..n-1].
void ParallelAverage( float* output, const float* input, size_t n ) {
    Average avg;
    avg.input = input;
    avg.output = output;
    parallel_for( blocked_range<int>( 1, n ), avg );
}

int main()
{
    int N = 10;
    float* a = new float[N]; //{5, 6, 2, 8, 4, 0, 1, 45};//new float[N];
    for (int i = 0; i < N; i++)
        a[i] = i;
    float* buf = new float[N];
    for (size_t i = 0; i < N; i++) cout << a[i] << ' ';
    cout << endl;
    cout << endl;
    ParallelAverage(buf, a, N);
    cout << endl;
    cout << endl;
    for (size_t i = 0; i < N; i++) cout << buf[i] << ' ';
}
