#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

/**
  sys_getppid - Recibe el ID del proceso padre.
  Retorna el ID del proceso padre.
**/
uint64
sys_getppid(void)
{
  struct proc *p = myproc();

  //  Si el proceso no tiene padre, retorna -1.
  if (p->parent == 0) {
    return -1;
  }

  // Retorna el ID del padre.
  return p->parent->pid;
}

/**
  sys_getancestor - Retorna el ID del ancestro en la posición
                    en la lista de procesos que ha creado el
                    proceso actual.
 
  Nota: El ancestro en la posición 0 es el proceso actual.
 
  Retorna el ID del ancestro solicitado o -1 si el proceso no
  tiene un ancestro en esa posición.
**/
uint64
sys_getancestor(void)
{
  int ancestor_level;
  struct proc *current_process = myproc();

  argint(0, &ancestor_level);

  // Si el nivel de ancestro es menor que 0, retorna -1.
  if (ancestor_level < 0) {
    return -1;
  }

  // Itera sobre la lista de procesos y busca el ancestro
  // en la posición solicitada.
  while (ancestor_level-- > 0) {
    if (current_process->parent == 0) {
      // Si el proceso no tiene padre en esa posición, retorna -1.
      return -1;
    }
    current_process = current_process->parent;
  }

  // Retorna el ID del ancestro solicitado.
  return current_process->pid;
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
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
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
