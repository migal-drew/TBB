#include <tbb/pipeline.h>
#include <stdio.h>
#include <math.h>

using namespace tbb;

float RootMeanSquare( float* first, float* last ) {
    float sum=0;
    parallel_pipeline( /*max_number_of_live_token=*/16,
        make_filter<void,float*>(
            filter::serial,
            [&](flow_control& fc)-> float*{
                if( first<last ) {
                    return first++;
                 } else {
                    fc.stop();
                    return NULL;
                }
            }
        ) &
        make_filter<float*,float>(
            filter::parallel,
            [](float* p){return (*p)*(*p);}
        ) &
        make_filter<float,void>(
            filter::parallel,
            [&](float x) {sum+=x;}
        )
    );
    return sqrt(sum);
}

int main()
{
    //float first = 2;
    //float last  = 11;
    float arr[] = {1, 1, 1, 1};
    float ans = RootMeanSquare(arr, arr + 10);
    printf("Result is: %f\n", ans);
}
