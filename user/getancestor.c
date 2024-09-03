#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

/**
  @brief Imprime el ID del ancestro en el nivel dado.

  Crea una jerarquía de procesos en donde cada proceso hijo genera otro
  proceso hijo, creando una cadena de procesos. Luego, el último proceso
  hijo llama a la función test_getancestor() para verificar el ID del
  ancestro en el nivel dado.

  @param level El nivel del ancestro a buscar.
**/
void
test_getancestor(int level)
{
  int apid = getancestor(level);
  if (apid >= 0) {
    printf("El ancestro con nivel %d tiene el PID %d\n", level, apid);
  } else {
    printf("No existe un ancestro con nivel %d\n", level);
  }
}

/**
  @brief Ejecuta el programa getancestor().

  El programa getancestor ejecuta el testeo de la llamada al sistema getancestor().

  @param argc El número de argumentos pasados al programa.
  @param argv Los argumentos pasados al programa.
**/
int
main(int argc, char *argv[])
{
  int ancestors;

  // Verifica que se haya pasado exactamente un argumento-
  if (argc != 2) {
    printf("Usage: %s <number_of_ancestors>\n", argv[0]);
    exit(1);
  }

  // Convierte el argumento en un entero.
  ancestors = atoi(argv[1]);

  // Crea una jerarquía de procesos.
  for (int i = 0; i < 2; i++) {
    int pid = fork();
    if (pid < 0) {
      // Si falla el fork, imprime un mensaje y sale.
      printf("Fork failed\n");
      exit(1);
    } else if (pid == 0) {
      // Si el PID es cero, se trata del proceso hijo, por lo que.
      // se sale del bucle y se sigue ejecutando el programa.
      continue;
    } else {
      // Si el PID es distinto de cero, se trata del proceso padre,
      // por lo que se espera a que el proceso hijo termine.
      wait(0);
      exit(0);
    }
  }

  // Verifica que el proceso actual no sea el proceso raíz.
  if (getpid() != 0) {
    // Imprime el ID del proceso actual
    printf("ID del proceso actual: %d\n", getpid());
    // Llama a la función test_getancestor con el argumento pasado.
    test_getancestor(ancestors);
    exit(0);
  }

  return 0;
}