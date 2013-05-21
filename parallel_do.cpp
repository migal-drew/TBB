#include <stdio.h>
#include <stdlib.h>
#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/partitioner.h"
#include "tbb/parallel_do.h"
#include "tbb/task.h"
#include "tbb/task_group.h"
#include <list>

using namespace std;
using namespace tbb;

class Foo
{
public:
    void operator()(int& m) const
    {
        printf("%d ", m);
    }
};

int main(int argc, char* argv[])
{
    task_scheduler_init init;
    list<int> lst;
    int n = 101;
    for (int i = 0; i < n; i++)
        lst.push_back(i);

    tbb::parallel_do(lst.begin(), lst.end(), Foo());
    printf("\n");
}
