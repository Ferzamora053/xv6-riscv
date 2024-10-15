# Informe Tarea 2
Nombre: Fernando Zamora\
Curso: Sistemas operativos\
Sección: 1\
Profesor: Sebastián Saez

## 1. Funcionamiento y lógica del sistema de prioridades.
Para esta tarea, se solicitó cambiar el programdor de procesos del sistema operativo xv6 a uno basado en prioridades con boost, donde hay que cumplir con los siguientes requerimientos:
- Cada proceso se inicia con prioridad 0 y boost 1.
- Cada vez que un proceso ingresa al programador de procesos, hay que aumentar la prioridad de todos los demás procesos existentes que puedan ser ejecutados (no zombies).
- La prioridad aumenta con la siguiente lógica: `Prioridad += Boost`.
- Si la prioridad de un proceso alcanza 9, cambiar el valor del boost a -1.
- Si la prioridad de un proceso llega a 0, cambiar el valor del boost a 1.

Además, será necesario crear un programa de prueba, el cuál tiene que cumplir con lo siguiente:
- Utilice `fork()` para crear al menos 20 procesos.
- Cada proceso debe mostrar un mensaje en pantalla al ejecutarse, con el formato: "Ejecutando proceso `<nombre>` `<pid>`".
- Incluya un `sleep` para hacer que los procesos se detengan unos segundos.

### Implementación del programador de procesos.
Para cambiar el programador de procesos del Sistema Operativo (SO) xv6-riscv, hay que cambiar la función `void scheduler(void)` que se encuentra en el archivo `proc.c` en el directorio del kernel `~/xv6-riscv/kernel/`. Además, si queremos que nuestro programador de procesos sea uno que cumpla con los requisitos anteriormente mencionados, hay que modificar la función `static struct proc* allocproc(void)` y el archivo `proc.h`.

En el archivo `proc.h` agregamos los atributos de `priority` y `boost` a la estrucutra del proceso (`struct proc`), para almacenar la prioridad y el boost de éste. Definimos ambas variables como un entero, con el fin de implementar con mayor facilidad la lógica de aumento y decrecimiento de la prioridad.

Luego de definir los nuevos atributos, nos toca modificar el archivo `proc.c` para implementar un scheduler basado en prioridades. En primer lugar, inicializamos los atributos anteriores en la función `static struct proc* allocproc(void)`, cuyo trabajo es incializar un proceso que esté en el estado de `UNUSED`. Agregamos las siguientes lineas en la función:
```c
p->priority = 0;
p->boost = 1;
```
Se realiza esto, para que cuando un proceso pase de `UNUSED` a `USED`, los valores de priority y boost sean 0 y 1, respectivamente, de esta manera, logramos que cada vez que un proceso se inicie tenga prioridad 0 y boost 1.

Por otro lado, la implementación del programador de procesos, se realiza modificando la función `void scheduler(void)`. Antes de explicar el funcionamiento de éste, se mostrará la función completa modficada.
```c
void
scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();

  c->proc = 0;
  for(;;){
    // The most recent process to run may have had interrupts
    // turned off; enable them to avoid a deadlock if all
    // processes are waiting.
    intr_on();

    struct proc *high_p = 0;

    // Find the process with the highest priority
    for (p = proc; p < &proc[NPROC]; p++) {
      acquire(&p->lock);
      if (p->state == RUNNABLE) {
        // Aumentar la prioridad de todos los procesos existentes en 1.
        p->priority += p->boost;

        // Si la prioridad es mayor o igual a 9, se cambia el boost a -1.
        if (p->priority >= 9) {
          p->boost = -1;
        }

        // Si la prioridad es menor o igual a 0, se cambia el boost a 1.
        else if (p->priority <= 0) {
          p->boost = 1;
        }

        // Si no hay un proceso con prioridad alta o la prioridad del proceso actual
        // es menor a la prioridad del proceso con prioridad alta.  
        // Se actualiza la prioridad del proceso con prioridad alta.
        if (high_p == 0 || p->priority < high_p->priority) {
          high_p = p;
        }
      }
      release(&p->lock);
    }

    // Si hay un proceso con prioridad alta
    // se cambia el estado a RUNNING y se ejecuta.
    if (high_p != 0) {
      acquire(&high_p->lock);
      if (high_p->state == RUNNABLE) {
        high_p->state = RUNNING;
        c->proc = high_p;
        swtch(&c->context, &high_p->context);

        c->proc = 0;
      }
      release(&high_p->lock);
    } else {
      intr_on();
      asm volatile("wfi");
    }
  }
}
```
Comparando el nuevo programador de procesos con el anterior, podemos notar que las diferencias empiezan luego de la llamada a la función `intr_on()`. Lo primero que hacemos es definir un proceso auxiliar denominado `high_p`, para poder determinar cuál es el proceso con mayor prioridad. Luego, iniciamos un ciclo `for` que recorre toda la tabla de procesos, comparando las prioridades de los procesos, y, así, determinado el que tenga mayor prioridad. Antes de hacer cambios en los valores de las variables, adquirimos el `lock` del proceso iterado y realizamos una comparación en relación al estado de éste. Si el estado del proceso es `RUNNABLE`, entonces, aumentamos en el valor del boost la prioridad de todos los demás procesos. Posteriormente, definimos las condiciones de los casos de eje, en donde, si la prioridad de un proceso es mayor o igual a 9, entonces, el valor del boost se cambia a -1 y si la prioridad es menor o igual a 0, entonces, se vuelve a cambiar el valor del boost a 1. Lo último que se realiza en el ciclo for, es determinar si el proceso iterado es el que tiene mayor prioridad. Esto lo logramos con la siguiente condición: `high_p == 0 || p->priority < high_p->priority`, la cuál revisa si no hay ningún proceso con que ha sido etiquetado como el de mayor prioridad o, si el proceso iterado tiene una prioridad mayor al proceso con mayor prioridad histórico. Cabe destacar, que un menor valor en el atributo de prioridad implica que la prioridad del proceso es mayor, por lo tanto, por eso se ocupa el operador menor que (`<`) envés de el operador mayor que (`>`). Una vez se hayan realizado todos los checkeos de las condiciones, liberamos el `lock` del proceso, para permitir que otro pueda acceder a la sección crítica.

Una vez, realizado el ciclo `for` para determinar el proceso de mayor prioridad, adquirimos un `lock` para éste y cambiamos su estado de `RUNNABLE` a `RUNNING`, cambiamos el proceso que está ocupando la CPU a éste y realizamos un context switch con `swthc(&c->context, &high_p->context)`. Cuando el proceso termine de ejecutarse, cambiamos el proceso de la CPU a uno vacío para permitir que otro proceso la ocupe y liberamos el `lock` del proceso `high_p`. En caso de que no haya algún proceso en estado `RUNNABLE`, habilitamos las interrupciones con `intr_on()` para evitar bloqueos, y el procesador entra en un estado de espera eficiente con la instrucción `wfi` hasta que ocurra una interrupción que despierte al scheduler, evitando así el desperdicio de ciclos de CPU.

### Programa de prueba.
Para probar el correcto funcionamiento de nuestro scheduler, se creó el siguiente programa de prueba:
```c
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
      sleep(5);
      wait(0);
    }
  }

  exit(0);
}
```
Este programa empieza creando 20 procesos hijos utilizando un ciclo `for` en donde cada iteración se hace una llamada al sistema `fork()`. Según el resultado que retorne la función `fork()`, se utilizan 3 condicionales para manejar la creación de los procesos hijos. Si la función retorna un valor menor a 0, entonces, ha fallado el proceso de creación de un hijo, por lo que, el programa imprime un mensaje de error y termina con un código de salida de 1. En el caso que la función retorne un 0, entonces, se ha creado un proceso hijo y nos encontramos en éste, por lo que se imprime un mensaje con el número del proceso (donde se prefiere empezar por 1 envés de 0, por lo que se ocupa `i + 1`), el id del proceso con la llamada al sistema `getpid()`, la prioridad del proceso con la llamada al sistema `getpriority()` y el valor del boost del proceso con la llamada al sistema `getboost()`. Luego, el proceso hijo termina con `exit(0)`. Por último, si el valor que retorne la función es mayor a 0, entonces, nos encontramos en el proceeso padre, por lo que, este, primero, espera a que el hijo termine y luego se duerme durante 5 tiempos. El resultado del programa se puede visualizar en la siguiente imagen.



## 2. Explicación de las modificaciones realizadas
Para implementar un scheduler basado en prioridades, se hicieron modificaciones en los siguientes archivos `syscall.h`, `syscall.c`, `user.h`, `usys.pl`, `proc.c`, `proc.h`, `sysproc.c` y `Makefile`, además del archivo de prueba en la carpeta `xv6-riscv/user/`.

- `syscall.h`
  Se agregaron las siguientes líneas al final del archivo para definir las llamadas, con el fin de que el kernel pueda identificar cuál llamada es la que se está invocando.
  ```c
  #define SYS_getpriority 22
  #define SYS_getboost 23
  ```
- `syscall.c`
  Se agregaron las siguientes líneas al final del bloque `extern ...`
  ```c
  extern uint64 sys_getpriority(void);
  extern uint64 sys_getboost(void);
  ```
  y al final de la función `static uint64 (*syscalls[])(void)`
  ```c
  [SYS_getpriority] sys_getpriority,
  [SYS_getboost] sys_getboost,
  ```
  Se hacen estas modificaciones para mapear los números de llamadas al sistema a sus correspondientes funciones en el kernel.

- `user.h`
  Se agregaron las siguientes líneas al final del bloque de llamadas del sistema para poder usar las llamadas al sistema de forma similar a cómo se llaman otras funciones de la biblioteca estándar.
  ```c
  int getpriority(void);
  int getboost(void);
  ```
- `usys.pl`
  Se agregaron las siguientes líneas al final del archivo con el fin de permitir que las llamadas sean accesibles desde el espacio del usuario.
  ```c
  entry("getpriority");
  entry("getboost");
  ```
- `proc.c`
  La funciones modificadas se muestran a continuación. La explicación de los cambios se pueden encontrar en la sección de implementación del programador de procesos.
  - `allocproc()`
    ```c
    static struct proc*
    allocproc(void)
    {
      struct proc *p;

      for(p = proc; p < &proc[NPROC]; p++) {
        acquire(&p->lock);
        if(p->state == UNUSED) {
          goto found;
        } else {
          release(&p->lock);
        }
      }
      return 0;

    found:
      p->pid = allocpid();
      p->state = USED;
      p->priority = 0;
      p->boost = 1;

      // Allocate a trapframe page.
      if((p->trapframe = (struct trapframe *)kalloc()) == 0){
        freeproc(p);
        release(&p->lock);
        return 0;
      }

      // An empty user page table.
      p->pagetable = proc_pagetable(p);
      if(p->pagetable == 0){
        freeproc(p);
        release(&p->lock);
        return 0;
      }

      // Set up new context to start executing at forkret,
      // which returns to user space.
      memset(&p->context, 0, sizeof(p->context));
      p->context.ra = (uint64)forkret;
      p->context.sp = p->kstack + PGSIZE;

      return p;
    }
    ```
  - `scheduler()`
    ```c
    void
    scheduler(void)
    {
      struct proc *p;
      struct cpu *c = mycpu();

      c->proc = 0;
      for(;;){
        // The most recent process to run may have had interrupts
        // turned off; enable them to avoid a deadlock if all
        // processes are waiting.
        intr_on();

        struct proc *high_p = 0;

        // Find the process with the highest priority
        for (p = proc; p < &proc[NPROC]; p++) {
          acquire(&p->lock);
          if (p->state == RUNNABLE) {
            // Aumentar la prioridad de todos los procesos existentes en 1.
            p->priority += p->boost;

            // Si la prioridad es mayor o igual a 9, se cambia el boost a -1.
            if (p->priority >= 9) {
              p->boost = -1;
            }

            // Si la prioridad es menor o igual a 0, se cambia el boost a 1.
            else if (p->priority <= 0) {
              p->boost = 1;
            }

            // Si no hay un proceso con prioridad alta o la prioridad del proceso actual
            // es menor a la prioridad del proceso con prioridad alta.  
            // Se actualiza la prioridad del proceso con prioridad alta.
            if (high_p == 0 || p->priority < high_p->priority) {
              high_p = p;
            }
          }
          release(&p->lock);
        }

        // Si hay un proceso con prioridad alta
        // se cambia el estado a RUNNING y se ejecuta.
        if (high_p != 0) {
          acquire(&high_p->lock);
          if (high_p->state == RUNNABLE) {
            high_p->state = RUNNING;
            c->proc = high_p;
            swtch(&c->context, &high_p->context);

            c->proc = 0;
          }
          release(&high_p->lock);
        } else {
          intr_on();
          asm volatile("wfi");
        }
      }
    }
    ```
- `proc.h`
  Se agregaron los atributos de `priority` y `boost` a la estructura del proceso y queda como:
  ```c
  struct proc {
    struct spinlock lock;

    // p->lock must be held when using these:
    enum procstate state;        // Process state
    void *chan;                  // If non-zero, sleeping on chan
    int killed;                  // If non-zero, have been killed
    int xstate;                  // Exit status to be returned to parent's wait
    int pid;                     // Process ID

    // wait_lock must be held when using this:
    struct proc *parent;         // Parent process

    // attributes for priority based scheduling
    int priority;                // priority of the process
    int boost;                   // boost of the process

    // these are private to the process, so p->lock need not be held.
    uint64 kstack;               // Virtual address of kernel stack
    uint64 sz;                   // Size of process memory (bytes)
    pagetable_t pagetable;       // User page table
    struct trapframe *trapframe; // data page for trampoline.S
    struct context context;      // swtch() here to run process
    struct file *ofile[NOFILE];  // Open files
    struct inode *cwd;           // Current directory
    char name[16];               // Process name (debugging)
  };
  ```
- `sysproc.c`
  Se agregaron las siguientes funciones en el archivo, para poder mostrar la prioridad y el boost de cada proceso.
  ```c
  uint64
  sys_getpriority(void)
  {
    return myproc()->priority;
  }

  uint64
  sys_getboost(void)
  {
    return myproc()->boost;
  }
  ```
- `Makefile`
  Se agregaron las siguientes líneas al final de la variable `UPROGS` para permitir que los arhivos de testeo sean compilados e incluidos en el kernel.
  ```Makefile
  $U/_getpid\
  $U/_getpriority\
  $U/_prioritytest\
  ```

Todos estos cambios se realizaron con el fin de poder utilizar las llamadas al sistema `getpriority()` y `getboost()` en el programa de testeo.

## 3. Dificultades encontradas y cómo se resolvieron