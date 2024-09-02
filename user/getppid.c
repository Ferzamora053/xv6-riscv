#include "kernel/types.h"
#include "user/user.h"

/**
  @brief Testear la llamada al sistema getppid().
 
  Ejecutamos un fork de un proceso, obtenemos el ID del proceso padre
  con getppid() y lo imprimimos.
**/
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
    printf("El proceso con ID %d tiene como padre al proceso con ID: %d\n", pid, ppid);
    // Salimos el proceso hijo.
    exit(0);
  }
  
  // Padre.
  else {
    // Esperamos a que el proceso hijo termine.
    wait(0);
  }
}


int
main()
{
  test_getppid();
  exit(0);
}