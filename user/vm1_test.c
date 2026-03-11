#include "kernel/types.h"   // xv6 基础类型定义
#include "user/user.h"      // xv6 用户态库（包含 printf、getpid 等）

#define PGSIZE 4096

struct  usyscall
{
   int pid;
};

int main(){
    long int a=((((1L << (9 + 9 + 9 + 12 - 1)) - PGSIZE) - PGSIZE) - PGSIZE);
    struct usyscall*usc=(struct usyscall*) a;
    printf("pid：%d\n",usc->pid);
    return 0;
}