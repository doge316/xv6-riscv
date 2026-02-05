#include"kernel/types.h"
#include"user.h"

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(2,"usage: sleep [ticks num]\n");
        exit(1);
    }
    int sleeptime=atoi(argv[1]);
    int ret=sleep(sleeptime);
    exit(ret);
}