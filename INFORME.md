# Informe Tarea 1
Nombre: Fernando Zamora\
Curso: Sistemas operativos\
Sección: 1\
Profesor: Sebastián Saez


## 1. Funcionamiento de las llamadas al sistema.
  #### **Implementación de la llamada de getppid(void).**

  Para implementar la llamada de sistema al sistema que retorne el ID del proceso padre del proceso que la invoca, se analizó el funcionamiento de la llamada al sistema que retorna el ID del proceso actual (**getpid()**). Esta llamada se encuentra definida en el archivo sysproc.c y tiene la siguiente forma
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
  ![resultado getpid()](images/resultado-getpid.png).

  Podemos modificar la llamada `sys_getpid(void)` de la siguiente manera, para que retorne el ID del proceso padre.
  ```c
  uint64
  sys_getppid(void)
  {
    struct proc *p = myproc();

    // Si el padre no existe, retorna -1.
    if (p->parent == 0) {
      return -1;
    }

    // Retorna el ID del padre.
    return p->parent->pid;
  }
  ```
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
  Este código empieza creando un fork de un proceso con `child = fork()` para guardar el valor de la función `fork()`. Si el valor de esta función es 0, entonces, estamos en el proceso hijo; si es mayor a 0, estamos en el padre; y si es menor a 0, entonces, la llamada a `fork()` falló.
  En primer lugar, revisamos con un `if statement` si ha fallado el `fork()` y salimos con código de salida 1.
  En segundo lugar, nos encontramos con otro `if statement` que revisa si el valor de `child` es igual a 1, Se cumple esta condición si estamos trabajando con el proceso hijo, donde guardamos las llamdas al sistema `getpid()` y `getppid()` en las variables `pid` y `ppid`, imprimimos sus valores y terminamos el proceso hijo con código de salida 0.
  Por último, tenemos un `else statement` que asegura que los procesos padres esperen que sus hijos termine para una finalización ordenada. <br>
  Al ejecutar este código, obtenemos el siguiente resultado
  ![resultado getppid](images/resultado-getppid.png)
  <br>

  #### **Implementación de la llamada getancestor()**

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