# Informe Tarea 0
Nombre: Fernando Zamora <br>
Curso: Sistemas operativos <br>
Sección: 1 <br>
Profesor: Sebastián Saez <br>


## 1. Pasos seguidos para instalar xv6
- Instalar la versión 7.2.0 de qemu y compilarlo para riscv64-softmnu. <br>
Link del repositorio de qemu: https://gitlab.com/qemu-project/qemu.git y la rama de la versión 7.2.0. <br>
Link de descarga de qemu: https://www.qemu.org/download/#linux.
- Instalar el riscv-gnu-toolchain y compilar para newlib
- Compilar xv6-riscv

## 2. Problemas encontrados y soluciones
- Problema: La versión 6.2.0 de qemu no compila correctamente el xv6-riscv. <br>
  Solución: Instalar la versión 7.2.0

- Problema: No se encuentra una versión de riscv64 de GCC/binutils <br>
  Solución: Instalar y compilar el toolchain

- Problema: No se encuentra una versión de un ejecutable QEMU funcional luego de compilar el QEMU para riscv64-softmnu <br>
  Solución: Instalar qemu con el siguiente comando: sudo apt-get install qemu-system

- Problema: -display gtk. Parámetro 'type' no acepta el valor 'gtk'. <br>
  Solución: Cambiar el parámetro a '-nographic' en el Makefile del repositorio del xv6-riscv. (Probablemente funcione con '-displaygtk', pero esto es solo para mostrar una ventana adicional)

## 3. Confirmación de que xv6 está funcionando correctamente.
