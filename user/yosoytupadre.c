#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

void
test_getppid(void)
{
  int child, pid, ppid;
  
  // Ejecutamos un fork.
  child = fork();
  
  if (child < 0) {
    printf("Fork fallido\n");
    exit(1);
  }

  // Hijo.
  if (child == 0) {
    // Obtenemos el PID del proceso padre.
    pid = getpid();
    ppid = getppid();
    // Imprimimos el ID del proceso padre.
    printf("Hola! Soy el proceso con ID %d y mi padre tiene el ID %d\n", pid, ppid);
    printf("Yo soy tu padre! - dijo el proceso con ID %d al proceso con ID %d\n", ppid, pid);
    // Salimos el proceso hijo.
    exit(0);
  }
  
  // Padre.
  else {
    // Esperamos a que el proceso hijo termine.
    wait(0);
  }
}

void
test_getancestor(int level)
{
  int apid = getancestor(level);
  if (apid >= 0) {
    printf("Mi ancestro de nivel %d tiene el PID %d\n", level, apid);
  } else {
    printf("No tengo un ancestro de nivel %d :(\n", level);
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
    printf("Hola! Soy el proceso con ID %d\n", getpid());
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