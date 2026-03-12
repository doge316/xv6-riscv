#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"
#include "sysinfo.h"

uint64 acquire_freemem();
uint64 acquire_nproc();

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(t == SBRK_EAGER || n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    if(addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//add a sys_trace function
uint64
sys_trace(void)
{
int mask;
argint(0,&mask);//读取a0寄存器的值,也就是要跟踪的系统调用号
struct proc *p = myproc();
p->trace_mask=mask;
return 0;
}

uint64
sys_sysinfo(void)
{
  uint64 addr;
  struct sysinfo info;
  struct proc *p=myproc();

  info.nproc=acquire_nproc();
  info.freemem=acquire_freemem();

  argaddr(0, &addr);//把a0存的info地址传给addr
  
  if(copyout(p->pagetable, addr, (char *)&info, sizeof(info)) < 0)//把内核态的info内容传给addr指向的内存
    return -1;

  return 0;
}

int 
sys_pgaccess(void)
{
  uint64 addr;
  int len;
  int bitmask;
  argaddr(0,&addr);//需要检查的地址
  argint(1,&len);//页数
  argint(2,&bitmask);//用户传入结果掩码地址
  if(len>32||len<0) return -1;
  
  int res=0;
  struct proc *p=myproc();
  for(int i=0;i<len;i++){
    int va=addr+i*PGSIZE;
    int abit=vm_pgaccess(p->pagetable,va);
    res=res|abit<<i;
  }
  if(copyout(p->pagetable,bitmask,(char*)&res, sizeof(res))<0)
    return -1;
  
  return 0;
}