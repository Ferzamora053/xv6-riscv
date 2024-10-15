#include "kernel/types.h"
#include "user/user.h"

int main()
{
  for (int i = 0; i < 20; i++) { // it has to run 20 processes
    int pid = fork();

    if (pid < 0) {
      printf("Fork failed\n");
      exit(1);
    } else if (pid == 0) {
      printf("Ejecutando proceso %d con pid %d, prioridad %d y boost %d\n", i + 1, getpid(), getpriority(), getboost());
      exit(0);
    } else {
      wait(0);
      sleep(5);
    }
  }

  exit(0);
}