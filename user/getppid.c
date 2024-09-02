#include "kernel/types.h"
#include "user/user.h"

void
test_getppid()
{
  int pid, ppid;

  pid = fork();
  
  if (pid == 0) {
    ppid = getppid();
    printf("El proceso con ID %d tiene como padre al proceso con ID: %d\n", getpid(), ppid);
    exit(0);
  } else if (pid > 0) {
    wait(0);
    printf("El proceso con ID %d tiene como hijo  al proceso con ID: %d\n", getpid(), pid);
  } else {
    printf("Fork failed\n");
    exit(1);
  }
}

int
main()
{
  test_getppid();
  exit(0);
}