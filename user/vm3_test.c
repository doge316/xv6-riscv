#include "kernel/types.h"   // xv6 基础类型定义
#include "user/user.h"      // xv6 用户态库（包含 printf、getpid 等）

#define PGSIZE 4096
int main(int argc, char**argv){
    char*buf; //以字节为单位的指针。如果是uint64就是以8字节为单位。
    unsigned int abits;
    buf=malloc(32*PGSIZE);
    if(pgaccess(buf,32,&abits)<0){
        printf("error in vm3_test\n");
        return 0;
    }
    buf[PGSIZE*1]+=1;
    buf[PGSIZE*2]+=1;
    buf[PGSIZE*30]+=1;
    if(pgaccess(buf,32,&abits)<0){
        printf("error in vm3_test\n");
        return 0;
    }
    if(abits==(1<<30|1<<2|1<<1))
        printf("vm3_test return true!\n");

    free(buf);
    return 0; 
}