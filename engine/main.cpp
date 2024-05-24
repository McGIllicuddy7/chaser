#include "runtime.h"
#include <gperftools/profiler.h>
int main(void){
    Runtime * runtime = Runtime::New();
    //ProfilerStart("output_inside.prof");
    runtime->Run();
    //ProfilerStop();
    delete runtime;
}