#include <raylib.h>
#include "runtime.h"
int main(void){
    Runtime * runtime = Runtime::New();
    runtime->Run();
}