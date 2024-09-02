#include "kernel/types.h"
#include "kernel/param.h"
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
    printf("El proceso con ID %d tiene como hijo al proceso con ID: %d\n", getpid(), pid);
  } else {
    printf("Fork fallado\n");
    exit(1);
  }
}

void
test_getancestor(int level)
{
  int apid = getancestor(level);
  if (apid >= 0) {
    printf("Ancestor level %d: PID %d\n", level, apid);
  } else {
    printf("Ancestor level %d: No such ancestor\n", level);
  }
}

void
run_tests(int level)
{
  printf("Ejecutando getppid:\n");
  test_getppid();

  printf("\nEjecutando getancestor:\n");

  int pid;
  // Cadena de forks para crear una jerarquía de procesos
  for (int i = 0; i < 2; i++) {
    pid = fork();
    if (pid > 0) {
      // El proceso padre espera a que el hijo termine
      wait(0);
      exit(0); // Salir una vez que el hijo haya terminado para mantener la jerarquía
    } else if (pid == 0) {
      // El proceso hijo continúa el bucle
      continue;
    } else {
      // Fork fallido
      printf("Fork fallado\n");
      exit(1);
    }
  }

  // En el último proceso hijo, se prueba getancestor
  if (pid == 0) {
    printf("ID del proceso actual: %d\n", getpid());
    test_getancestor(level);
    exit(0);
  }
}

int
main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Usage: %s <number_of_ancestors>\n", argv[0]);
    exit(1);
  }

  int level = atoi(argv[1]);
  run_tests(level);

  exit(0);
}