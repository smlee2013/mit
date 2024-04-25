#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
//add
void *memcpy(void *dst, const void *src, uint n);

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
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
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  backtrace();
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
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

uint64
sys_sigalarm(void)
{
   //printf("sigalarm says hi\n");
   int ticks;
   uint64 handler;
   
   argint(0, &ticks);
   argaddr(1, &handler);
   
   struct proc *p = myproc();
   p->ticks = ticks;
   p->handler = handler;
   p->ticks_cnt = 0;
   
   
   return 0;
}

void 
restore()
{
  struct proc * p = myproc();
  /*  40 */   p->trapframe->ra = p->tra;
  /*  48 */   p->trapframe->sp = p->tsp;
  /*  56 */   p->trapframe->gp = p->tgp;
  /*  64 */   p->trapframe->tp = p->ttp;
  /*  72 */   p->trapframe->t0 = p->tt0;
  /*  80 */   p->trapframe->t1 = p->tt1;
  /*  88 */   p->trapframe->t2 = p->tt2;
  /*  96 */   p->trapframe->s0 = p->ts0;
  /* 104 */   p->trapframe->s1 = p->ts1;
  /* 112 */   p->trapframe->a0 = p->ta0;
  /* 120 */   p->trapframe->a1 = p->ta1;
  /* 128 */   p->trapframe->a2 = p->ta2;
  /* 136 */   p->trapframe->a3 = p->ta3;
  /* 144 */   p->trapframe->a4 = p->ta4;
  /* 152 */   p->trapframe->a5 = p->ta5;
  /* 160 */   p->trapframe->a6 = p->ta6;
  /* 168 */   p->trapframe->a7 = p->ta7;
  /* 176 */   p->trapframe->s2 = p->ts2;
  /* 184 */   p->trapframe->s3 = p->ts3;
  /* 192 */   p->trapframe->s4 = p->ts4;
  /* 200 */   p->trapframe->s5 = p->ts5;
  /* 208 */   p->trapframe->s6 = p->ts6;
  /* 216 */   p->trapframe->s7 = p->ts7;
  /* 224 */   p->trapframe->s8 = p->ts8;
  /* 232 */   p->trapframe->s9 = p->ts9;
  /* 240 */   p->trapframe->s10 = p->ts10;
  /* 248 */   p->trapframe->s11 = p->ts11;
  /* 256 */   p->trapframe->t3 = p->tt3;
  /* 264 */   p->trapframe->t4 = p->tt4;
  /* 272 */   p->trapframe->t5 = p->tt5;
  /* 280 */   p->trapframe->t6 = p->tt6;
}

uint64
sys_sigreturn(void)
{
   //printf("sigalarm says hi\n");
   struct proc * p = myproc();
   p->trapframe->epc = p->ticks_epc;
   restore();
   p->handler_running = 0;
   return 0;
}
