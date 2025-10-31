/*En esta practica nuestro objetivo es crear una minishell*/

#define _POSIX_SOURCE // caracteristicas POSIX
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

// cabeceras y estandares

#include "orden.h"
#include "procesos.h"

// cabeceras del proyecto

#define PROMPT "mish>" // constante

#define NUM_ORDENES_INTERNAS 3                       // constante
#define NUM_ORDENES_TOTAL (NUM_ORDENES_INTERNAS + 1) // constante

static void ejecutar_pwd(const struct orden *orden);
static void ejecutar_cd(const struct orden *orden);
static void ejecutar_jobs(const struct orden *orden);
static void ejecutar_orden_externa(const struct orden *orden);

static const char *ordenes[NUM_ORDENES_TOTAL] =
    {
        "pwd",
        "cd",
        "jobs",
        "externa"};

static void (*ejecutar[NUM_ORDENES_TOTAL])(const struct orden *orden) =
    {
        ejecutar_pwd,
        ejecutar_cd,
        ejecutar_jobs,
        ejecutar_orden_externa};

static void ejecutar_pwd(const struct orden *orden)
{
    char cwd[PATH_MAX];

    if (!comprobar_errores_internos(orden, 0, "cd [dir]"))
    {
        return;
    }

    if (getcwd(cwd, PATH_MAX) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("pwd");
    }
}

static void ejecutar_jobs(const struct orden *orden)
{

    if (!comprobar_ordenes_internas(orden, 0, "jobs"))
    {
        return;
    }

    mostrar_procesos_y_eliminar_terminados();
}

static void ejecutar_orden_externa(const struct orden *orden)
{

    pid_t pid;
    int estado;
    int en_segundo_plano;

    en_segundo_plano = (orden->plano != primero);

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        if (manejar_redirecciones(orden) != 0)
        {
            exit(exit_failure);
        }

        if (execvp(orden->args[0], (char *const *)orden->args) == -1)
        {
            fprintf(stderr, "%s: %s\n", orden->args[0], strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}

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

static void ejecutar_cd(const struct orden *orden)
{
    const char *directorio;

    if (!comprobar_errores_internos(orden, 1, "cd [dir]"))
    {
        return;
    }

    directorio->orden->args[1];
    if (directorio == NULL)
    {
        directorio = getenv("HOME");
        if (directorio == NULL)
        {
            fprintf(stderr, "cd : No se encontro la variable de entorno HOME\n");
            return;
        }
    }

    if (chdir(directorio) == -1)
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
