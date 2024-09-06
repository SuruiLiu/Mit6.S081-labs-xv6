#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

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
  return 0;
}


#ifdef LAB_PGTBL

//base is the first pte in the pagetable,len is the number of pte(must <=32 cause uint only have 32 bit)
int
sys_pgaccess(void){
  pagetable_t u_pt = myproc()->pagetable;
  uint64 fir_addr, mask_addr;
  int size;
  uint mask = 0;
  
  if(argaddr(0, &fir_addr) < 0)
    return -1;
  if(argint(1, &size) < 0)
    return -1;
  if(argaddr(2, &mask_addr) < 0)
    return -1;
  
  if(size > 32){return -1;}
  
  pte_t* fir_pte = walk(u_pt, fir_addr, 0);
  
  for(int i =0; i < size; i++){
    if((fir_pte[i]&PTE_A)&&(fir_pte[i]&PTE_V)){
      mask |= (1<<i);
      fir_pte[i] ^= PTE_A;
    }
  }
  
  if(copyout(u_pt, (uint64)mask_addr, (char*)&mask, sizeof(uint)) < 0)
    return -1;
  
  return 0; // success
  
}
#endif

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
