#include <stdio.h>
#include <stdlib.h>
#include "tbb/blocked_range.h"
#include "tbb/partitioner.h"
#include "tbb/parallel_reduce.h"
#include "tbb/task.h"

using namespace tbb;

struct FibRange
{
    int _n;

    FibRange(int n): _n(n) {}

    FibRange(FibRange& other, tbb::split)
    {
        _n = other._n - 2;
        other._n -= 1;
    }

    bool is_divisible() const { return _n > 1; }
    bool empty() const { return _n < 0; }
};

struct Fib
{
    int fsum_;

    Fib():
        fsum_(0) { }

    Fib(Fib& other, tbb::split):
        fsum_(0) { }

    int fib(int n)
    {
        if (n < 2) return 1;
        else return fib(n - 2) + fib(n - 1);
    }

    void operator() (FibRange& range)
    {
        fsum_ += fib(range._n);
    }

    void join(Fib& rhs)
    {
        fsum_ += rhs.fsum_;
    }
};

int main(int argc, char* argv[])
{
    int n = atoi(argv[1]);
    Fib f;
    //static affinity_partitioner ap;
    parallel_reduce(FibRange(n), f, auto_partitioner());
    int res = f.fsum_;
    printf("Result: %d \n", res);
}
