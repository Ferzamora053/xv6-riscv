#include "kernel/types.h"
#include "user/user.h"

int
main()
{
  int pid = getpid();
  printf("Process ID: %d\n", pid);
  exit(0);
}