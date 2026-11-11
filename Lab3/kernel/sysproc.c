#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

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
  backtrace();
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

uint64
sys_sigalarm(void)
{
  struct proc *p = myproc();
  argint(0, &(p->alarm_interval));
  argaddr(1, &(p->handler));
  return 0;
}

uint64
sys_sigreturn(void)
{
  struct proc *p = myproc();
  p->trapframe->kernel_satp = p->alarm_save->kernel_satp;   // kernel page table
  p->trapframe->kernel_sp = p->alarm_save->kernel_sp;   // kernel page table
  p->trapframe->kernel_trap = p->alarm_save->kernel_trap;   // kernel page table
  p->trapframe->epc = p->alarm_save->epc;   // kernel page table
  p->trapframe->kernel_hartid = p->alarm_save->kernel_hartid;   // kernel page table
  p->trapframe->ra = p->alarm_save->ra;   // kernel page table
  p->trapframe->sp = p->alarm_save->sp;   // kernel page table
  p->trapframe->gp = p->alarm_save->gp;   // kernel page table
  p->trapframe->tp = p->alarm_save->tp;   // kernel page table
  p->trapframe->t0 = p->alarm_save->t0;   // kernel page table
  p->trapframe->t1 = p->alarm_save->t1;   // kernel page table
  p->trapframe->t2 = p->alarm_save->t2;   // kernel page table
  p->trapframe->s0 = p->alarm_save->s0;   // kernel page table
  p->trapframe->s1 = p->alarm_save->s1;   // kernel page table
  p->trapframe->a0 = p->alarm_save->a0;   // kernel page table
  p->trapframe->a1 = p->alarm_save->a1;   // kernel page table
  p->trapframe->a2 = p->alarm_save->a2;   // kernel page table
  p->trapframe->a3 = p->alarm_save->a3;   // kernel page table
  p->trapframe->a4 = p->alarm_save->a4;   // kernel page table
  p->trapframe->a5 = p->alarm_save->a5;   // kernel page table
  p->trapframe->a6 = p->alarm_save->a6;   // kernel page table
  p->trapframe->a7 = p->alarm_save->a7;   // kernel page table
  p->trapframe->s2 = p->alarm_save->s2;   // kernel page table
  p->trapframe->s3 = p->alarm_save->s3;   // kernel page table
  p->trapframe->s4 = p->alarm_save->s4;   // kernel page table
  p->trapframe->s5 = p->alarm_save->s5;   // kernel page table
  p->trapframe->s6 = p->alarm_save->s6;   // kernel page table
  p->trapframe->s7 = p->alarm_save->s7;   // kernel page table
  p->trapframe->s8 = p->alarm_save->s8;   // kernel page table
  p->trapframe->s9 = p->alarm_save->s9;   // kernel page table
  p->trapframe->s10 = p->alarm_save->s10;   // kernel page table
  p->trapframe->s11 = p->alarm_save->s11;   // kernel page table
  p->trapframe->t3 = p->alarm_save->t3;   // kernel page table
  p->trapframe->t4 = p->alarm_save->t4;   // kernel page table
  p->trapframe->t5 = p->alarm_save->t5;   // kernel page table
  p->trapframe->t6 = p->alarm_save->t6;   // kernel page table
  p->alarm_ticks = 0;
  return p->trapframe->a0;
}