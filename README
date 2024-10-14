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

### Programa de prueba.


## 2. Explicación de las modificaciones realizadas
Para implementar un scheduler basado en prioridades, se hicieron modificaciones en los siguientes archivos `syscall.h`, `syscall.c`, `user.h`, `usys.pl`, `proc.c`, `proc.h`, `sysproc.c` y `Makefile`, además del archivo de prueba en la carpeta `xv6-riscv/user/`.

- `syscall.h`
Se agregaron las siguientes 

- `syscall.c`

- `user.h`

- `usys.pl`

- `proc.c`

- `proc.h`

- `sysproc.c`

- `Makefile`

## 3. Dificultades encontradas y cómo se resolvieron