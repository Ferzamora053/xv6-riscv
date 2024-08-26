# Informe Tarea 0
Nombre: Fernando Zamora\
Curso: Sistemas operativos\
Sección: 1\
Profesor: Sebastián Saez

## 1. Pasos seguidos para instalar xv6
Para instalar el sistema operativo xv6-riscv, se siguieron los siguientes pasos.
  <ol>
    <li> 
      <strong>
        Instalar WSL y Ubuntu en un computador Windows.
      </strong> <br>
      El sistema operativo xv6-riscv requiere de herramientas que no están disponibles en el sistema operativo de Windows 11, por lo que es necesario trabajar en una distribución de Linux o en MacOS. La solución más simple a este problema es instalar y utilizar Windows Subsystem for Linux (<strong>WSL</strong>), el cuál permite instalar diversas distribuciones de Linux como una máquina virtual. Ocupando el comando <br> <code>wsl --install</code>, se instala la versión 22.04 de Ubuntu. Luego, se nos pedirá crear un usuario, en donde hay que ingresar un nombre y una contraseña. Para acceder a la máquina virtual, ocupamos el comando <code>ubuntu</code> en la terminal de Windows y para salir, ocupamos <code>exit</code>.
      Por último, es necesario ejecutar el siguiente comando <code>sudo apt update -y && sudo apt upgrade -y</code> para actualizar los distintos paquetes de Ubuntu.
    </li><br>
    <li>
      <strong>
        Crear un directorio de trabajo para la instalación de las distintas herramientas.
      </strong> <br>
      Antes de instalar las distintas herramientas, crearemos un directorio para poder trabajar con los distintos repositorios. En nuestro caso, crearemos el directorio base <code>/sysops</code>. Con el comando <code>mkdir sysops</code> creamos una carpeta en el directorio <code>~/</code> y con <code>cd sysops</code> nos movemos a ésta. Por lo tanto, nuestra ruta en donde instalaremos los repositorios será <code>~/sysops</code>. <br>
    </li><br>
    <li>
      <strong>
        Instalar la versión 7.2.0 de QEMU y compilarlo para riscv64-softmnu.
      </strong> <br>
      Para realizar esto, hay que clonar el repositorio en git de <a ref="https://gitlab.com/qemu-project/qemu.git">QEMU</a> en la carpeta de trabajo (<code>~/sysops</code>), en conjunto con la rama de la versión 7.2.0. Para ello, ocuparemos el comando <code>git clone -b v7.2.0 https://gitlab.com/qemu-project/qemu.git</code>, el cuál clona el repositorio de QEMU y la rama de la versión 7.2.0. Para poder continuar con la instalación, debemos movernos a la carpeta clonada, y logramos esto con el comando <code>cd qemu</code>. Luego configuramos la build de QEMU con <code>./configure --target-list=riscv64-softmmu</code> y construimos QEMU con el comando <code>make -j$(nproc)</code>.
      Link de descarga de qemu: https://www.qemu.org/download/#linux.
    </li><br>
    <li>
      <strong>
        Instalar el riscv-gnu-toolchain y compilar para newlib. 
      </strong> <br>
    </li><br>
    <li>
      <strong>
        Compilar xv6-riscv.
      </strong> <br>
    </li>
  </ol>

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
![Requisitos xv6](images/Requisitos-xv6.png)
