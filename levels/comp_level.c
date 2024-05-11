#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, const char ** argv){
    if(argc<2){
        fprintf(stderr, "usage: ./comp_level <level file name>");
        return 1;
    }
    //char *const * args = {(char const *)argv[1], "-l", "../engine/engine.a", NULL};
    char * args[4];
    args[0] = (char *)argv[1];
    args[1] = "-c";
    args[2] = NULL;
    printf("%d\n",execvp("g++", args));
}