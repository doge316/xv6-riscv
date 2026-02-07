#include"kernel/types.h"
#include"user/user.h"

int main(int argc, char*argv[]){
    int fd[2];
    char buf[128];
    pipe(fd);
    int pid=fork();
    if(pid==0){
        close(fd[1]);//子读取，关闭写端stdout
        int n=read(fd[0],buf,sizeof(buf)-1);
        buf[n]='\0';
        printf("this is the connent: %s\n",buf);
        close(fd[0]);
        exit(0);
    }
    else{
        close(fd[0]);//父写入，关闭读端stdin
        write(fd[1],"hello",5);
        close(fd[1]);//只有当管道的所有写端都关闭才能开始读
        wait(0);
        exit(0);
    }
}