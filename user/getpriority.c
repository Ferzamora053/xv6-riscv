#include "kernel/types.h"
#include "user/user.h"

int
main()
{
  int priority = getpriority();
  printf("Process priority: %d\n", priority);
  exit(0);
}