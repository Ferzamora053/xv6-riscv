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
      sleep(3);
      exit(0);
    } else {
      wait(0);
    }
  }

  // Explicar que con una CPU el programa funciona como se espera, 
  // pero con más de una CPU, los prints se mezclan. Por eso se pone el wait adentro del for.
  // for (int i = 0; i < 20; i++) {
  //   wait(0);
  // }

  exit(0);
}