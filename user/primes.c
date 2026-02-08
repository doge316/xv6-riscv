#include "kernel/types.h"
#include "user/user.h"

void prime_filter(int read_fd){
    int prime, num;
    if(read(read_fd,&prime,sizeof(int))!=sizeof(int)){
        close(read_fd);
        exit(0);
    }
    printf("prime %d\n",prime);
    int p[2];
    pipe(p);
    while(read(read_fd,&num,sizeof(int))==sizeof(int)){
        if(num%prime!=0){
            write(p[1],&num,sizeof(int));
        }         
    }
    close(p[1]);
    close(read_fd);
    if(fork()==0){
        prime_filter(p[0]);
    }
    else{
        close(p[0]);
        wait(0);
    }
}

int main(){
    int p[2];
    pipe(p);
    int pid=fork();
    if(pid==0){
        close(p[0]);
        for(int i=2;i<=35;i++){
            write(p[1],&i,sizeof(int));
        }
        close(p[1]);
    }
    else{
        close(p[1]);
        prime_filter(p[0]);
        wait(0);
    }
}