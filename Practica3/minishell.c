/*En esta practica nuestro objetivo es crear una minishell*/
//tenemos que ejecutar comandos internos (pwd, cd,jobs)



#define _POSIX_SOURCE // caracteristicas POSIX
#include <stdio.h>     // Entrada/salida estándar
#include <string.h>    // Funciones de manejo de cadenas
#include <limits.h>    // Constantes como PATH_MAX
#include <stdlib.h>    // Funciones de utilidad general (malloc, exit, etc.)
#include <sys/wait.h>  // Para esperar procesos hijos
#include <sys/types.h> // Tipos de datos del sistema
#include <sys/stat.h>  // Estructuras y macros para ficheros
#include <fcntl.h>     // Control de archivos (open, O_* flags)
#include <unistd.h>    // Funciones POSIX (fork, exec, chdir, etc.)
#include <signal.h>    // Manejo de señales

// cabeceras del proyecto

#include "orden.h"
#include "procesos.h"


#define PROMPT "mish>" // Constante que define el texto del prompt de la shell

#define NUM_ORDENES_INTERNAS 3                       // Número de órdenes internas (pwd, cd, jobs)
#define NUM_ORDENES_TOTAL (NUM_ORDENES_INTERNAS + 1) // Total de órdenes (internas + externas)

static void ejecutar_pwd(const struct orden *orden);
static void ejecutar_cd(const struct orden *orden);
static void ejecutar_jobs(const struct orden *orden);
static void ejecutar_orden_externa(const struct orden *orden);

// Array con los nombres de las órdenes admitidas
static const char *ordenes[NUM_ORDENES_TOTAL] =
    {
        "pwd",
        "cd",
        "jobs",
        "externa"};

// Array de punteros a funciones que ejecutan cada orden
static void (*ejecutar[NUM_ORDENES_TOTAL])(const struct orden *orden) =
    {
        ejecutar_pwd,
        ejecutar_cd,
        ejecutar_jobs,
        ejecutar_orden_externa};

/*
 * Implementación de la orden interna "pwd"
 * Muestra el directorio actual de trabajo.
 */

static void ejecutar_pwd(const struct orden *orden)
{
    char cwd[PATH_MAX]; // Buffer para almacenar la ruta actual


        // Comprobamos validez
    if (!comprobar_errores_internos(orden, 0, "cd [dir]")) 
    {
        return;
    }

    if (getcwd(cwd, PATH_MAX) != NULL)     // Obtener el directorio actual y mostrarlo
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("pwd");
    }
}

 /* Implementación de la orden interna "jobs"
 * Muestra los procesos en segundo plano.
 */
static void ejecutar_jobs(const struct orden *orden)
{

    if (!comprobar_ordenes_internas(orden, 0, "jobs"))
    {
        return;
    }

    mostrar_procesos_y_eliminar_terminados();   // Función definida en procesos.h que muestra procesos activos
}


/*
 * Implementación de órdenes externas usando fork() y execvp().
 * Ejemplo: ls, cat, etc.
 */
static void ejecutar_orden_externa(const struct orden *orden)
{

    pid_t pid; 
    int estado;
    int en_segundo_plano;

    en_segundo_plano = (orden->plano != primero);     // Determinar si el comando se ejecutará en background

    //  Creamos un nuevo proceso con fork()
    //  - Devuelve un valor distinto en el padre y en el hijo
    //  - En el hijo, devuelve 0
    //  - En el padre, devuelve el PID del hijo

    pid = fork();  // Crear un proceso hijo

    if (pid < 0)
    {
         // Si fork falla (no se pudo crear el proceso), mostramos un error
        perror("fork");
        return;
    }
    else if (pid == 0)   // Proceso hijo
    {

         // Manejar redirecciones de entrada/salida si existen
        // Por ejemplo, si el usuario pone: "ls > salida.txt" o "cat < entrada.txt"
        if (manejar_redirecciones(orden) != 0) // Manejar redirecciones de entrada/salida
        {
            exit(exit_failure);  // Si algo falla al redirigir, el hijo termina con error
        }
      // Ejecutar el programa solicitado por el usuario
        // Usamos 'execvp', que reemplaza el proceso actual por el nuevo programa
        // - orden->args[0] es el comando (ej: "ls")
        // - orden->args es el array de argumentos terminados en NULL
        if (execvp(orden->args[0], (char *const *)orden->args) == -1)
        {
             // Si execvp() falla, mostramos el error correspondiente (por ejemplo, "comando no encontrado")
            fprintf(stderr, "%s: %s\n", orden->args[0], strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}


/* Función auxiliar que valida errores típicos de órdenes internas
 * - No deben ejecutarse en segundo plano
 * - No deben usar redirecciones
 * - No deben tener más de cierto número de argumentos
 */
static int comprobar_errores_internos(const struct orden *orden, int max_args, const char *uso)
{
    if (orden->plano != PRIMERO)
    {
        fprintf(stderr, "Error: la orden %s no puede ejecutarse en segundo plano\n", orden->args[0]);
        fprintf("Uso : %s\n", uso);
        return 0;
    }

    if (orden->entrada != NULL || orden->salida != NULL)
    {
        fprintf(stderr, "Error: la orden %s no puede tener redirecciones\n", orden->args[0]);
        fprintf(stderr, "Uso: %s\n", uso);
        return 0;
    }

    if (orden->args[max_args + 1] != NULL)
    {
        fprintf(stderr, "Demasiados argumentos. La orden %s debe tener como máximo %d argumentos\n", orden->args[0], max_args);
        fprintf(stderr, "Uso: %s\n" uso);
        return 0;
    }
    return 1;
}


/*
 * Función: ejecutar_cd
 * --------------------
 * Implementa la orden interna "cd", que cambia el directorio actual.
 * 
 * Si el usuario no indica un directorio, se cambia al directorio
 * indicado por la variable de entorno HOME.
 */
static void ejecutar_cd(const struct orden *orden)
{
    const char *directorio;  // Puntero a una cadena con el path destino

    // Comprobamos errores de uso con la función auxiliar
    //    - No puede redirigirse
    //    - No puede ejecutarse en background
    //    - Solo permite un argumento máximo (cd [directorio])

    if (!comprobar_errores_internos(orden, 1, "cd [dir]"))
    {
        return;
    }

  
 
    directorio->orden->args[1];  // Tomamos el segundo argumento (args[1]) como destino
    if (directorio == NULL)   // Si el usuario solo tecleó "cd", entonces args[1] será NULL
    {
        directorio = getenv("HOME");  //Si no se proporcionó directorio, usamos la variable de entorno HOME
        if (directorio == NULL)
        {
            fprintf(stderr, "cd : No se encontro la variable de entorno HOME\n");
            return;
        }
    }
        //Cambiar el directorio actual a la ruta indicada
    //    Si chdir() devuelve -1, hubo un error (por ejemplo, la carpeta no existe)
    if (chdir(directorio) == -1) //aqui lo que ocurre es que al no haber else si se ejecuta bien pues cambia al directorio
    {
        perror(directorio);
    }
    
}

static void ejecutar_orden(const struct orden *orden)
{
    int i;

    i = 0;
    while (i < NUM_ORDENES_INTERNAS && strcmp(ordenes[i], orden->args[0]))
    {
        i++;
    }
    ejecutar[i](orden);
}

int main(const int argc, const char *argv[])
{
    const struct orden *actual;

    if (argc > 1)
    {
        fprintf(stderr, "Uso: %s\n", argv[0]);
        return -1;
    }
    actual = leer_orden(PROMPT);
    while (strcmp("exit", actual->args[0]) != 0)
    {
        ejecutar_orden(actual);
        actual = leer_orden(PROMPT);
    }
    return 0;
}
