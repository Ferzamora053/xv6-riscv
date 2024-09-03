# Informe Tarea 1
Nombre: Fernando Zamora\
Curso: Sistemas operativos\
Sección: 1\
Profesor: Sebastián Saez


## 1. Funcionamiento de las llamadas al sistema.
  #### **Implementación de la llamada de getppid(void).**

  Para implementar la llamada de sistema al sistema que retorne el ID del proceso padre del proceso que la invoca, se analizó el funcionamiento de la llamada al sistema que retorna el ID del proceso actual (**getpid()**). Esta llamada se encuentra definida en el archivo `xv6-riscv/kernel/sysproc.c` y tiene la siguiente forma
  ```c
  uint64
  sys_getpid(void)
  {
    return myproc()->pid;
  }
  ```
  donde lo retornado el es ID del proceso. Cabe destacar, que esta llamada no se encuentra disponible por defecto, por lo que hay que agregar un script en C, que muestre por pantalla el ID del proceso. Un ejemplo de un script que utilice la llamda **getpid()** es el siguiente:
  ```c
  #include "kernel/types.h"
  #include "user/user.h"

  int
  main()
  {
    int pid = getpid();
    printf("Process ID: %d\n", pid);
    exit(0);
  }
  ```
  El cual retorna
  ![resultado getpid()](images/resultado-getpid.png)
  Podemos modificar la llamada `sys_getpid(void)` de la siguiente manera, para que retorne el ID del proceso padre.
  ```c
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
  ```
  En esencia, lo que se realiza es crear una estructura `proc` a la cuál revisamos si tiene padre. En caso de no tener, retornamos -1, y si tiene, retornamos el ID del padre con `p->parent->pid`.\
  Luego, creamos un archivo `.c` para testear si dicha llamada al sistema funciona como se solicita. A continuación se muestra un ejemplo de dicho archivo
  ```c
  #include "kernel/types.h"
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
  ```
  Este código empieza creando un fork de un proceso con `child = fork()` para guardar el valor de la función `fork()`. Si el valor de esta función es 0, entonces, estamos en el proceso hijo; si es mayor a 0, estamos en el padre; y si es menor a 0, entonces, la llamada a `fork()` falló.\
  En primer lugar, revisamos con un `if statement` si ha fallado el `fork()` y salimos con código de salida 1.\
  En segundo lugar, nos encontramos con otro `if statement` que revisa si el valor de `child` es igual a 1, Se cumple esta condición si estamos trabajando con el proceso hijo, donde guardamos las llamdas al sistema `getpid()` y `getppid()` en las variables `pid` y `ppid`, imprimimos sus valores y terminamos el proceso hijo con código de salida 0.\
  Por último, tenemos un `else statement` que asegura que los procesos padres esperen que sus hijos termine para una finalización ordenada. <br>
  Al ejecutar este código, obtenemos el siguiente resultado
  ![resultado getppid](images/resultado-getppid.png)

  #### **Implementación de la llamada getancestor(int)**
  La implementación de la llamada **getancestor(int)** es una extensión de la llamada **getppid(void)**, donde buscamos saber el ID de procesos más "antiguos" que el padre. A diferencia de la función anterior, ahora es necesario que **getancestor()** reciba como parámetro un número entero, el cuál representa el nivel del ancestro cuyo ID queremos saber. Es decir, si ejecutamos `getancestor(0)`, deberíamos esperar que el ID retornado sea el mismo proceso, si, ahora ejecutamos `getancestor(1)`, deberíamos esperar que el ID retornado sea el del padre del proceso y así consecutivamente con los demás valores (que, por como está definida la implementación, deben ser mayores a 0). <br>
  Para ello, debemos implementar la llamada en el archivo `sysproc.c` y se ve de esta forma:
  ```c
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
  ```
  Donde revisamos, primero, si el nivel ingresado es mayor a 0, que en caso contrario retornamos -1 y terminamos la llamada. En el caso que se cumpla que el valor ingresado sea mayor a 0, inicializamos un ciclo while que recorre los ancestor hasta llegar al nivel solicitado. Los padres de los procesos los guardamos en la variable `current_process` (estructura de tipo `proc`) y al finalizar el ciclo while, retornarmos `current_process->pid`.\
  Para probar si dicha llamada funciona, creamos un archivo `.c` para realizar el testeo. Un ejemplo de dicho archivo es el siguiente:
  ```c
  #include "kernel/types.h"
  #include "kernel/param.h"
  #include "user/user.h"

  /**
    @brief Imprime el ID del ancestro en el nivel dado.

    Si el ancestro en el nivel dado existe, imprime un mensaje con el
    ID del ancestro.  De lo contrario, imprime un mensaje indicando que
    el ancestro no existe.

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
  ```
  Este 

  <br>

  #### **Programa de prueba yosoytupadre.c**
  Imagen de la implementacion del programa.

## 2. Explicación de las modificaciones realizadas.

## 3. Dificultades encontradas y cómo se resolvieron.
  - **Problema: Reparentación de un proceso sin padre (Hacer fork)**\
    Solución:
  - **Problema: Reparentación de procesos con ancestros. (ni idea como solucionarlo).**\
    Solución:
  - **Problema: Cómo hacer múltiples fork a un proceso para tener múltiples ancestros.**\
    Solución: