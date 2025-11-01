#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "procesos.h"

/* estructura proceso con PID, ejecutable y estado */
struct proceso
{
    int br999_pid;                         /* declara un campo entero de nombre zz0000_pid donde zz0000
                                será tu matrícula */
    char br999_ejecutable[MAX_EJECUTABLE]; /* declara un campo de tipo array de caracteres de longitud MAX_EJECUTABLE de nombre zz0000_ejecutable */
    int br999_estado;                      /* declara un campo entero de nombre zz0000_estado */

    /**** PRÁCTICA ****/
};

/* función para añadir la información de un proceso al array procesos
   en la posición ix */
int anadir_proceso(struct proceso *procesos[], int ix, int pid,
                   char *ejecutable, int estado)
{

    struct proceso *nuevo; /* Declara un puntero a struct proceso */
    /**** PRÁCTICA ****/

    nuevo = calloc(1, sizeof(struct proceso)); /*
      1:estás reservando espacio para 1 estructura de tipo struct proceso.
  sizeof(struct proceso): calcula cuántos bytes ocupa esa estructura.
  calloc: reserva esa cantidad de memoria y la inicializa a cero.
  nuevo: es un puntero a struct proceso que apunta a esa memoria recién reservada. */

    if (nuevo == NULL)
    {
        fprintf(stderr, "Error en la reserva de memoria");
        return MEM_ERROR;
    }
    /* Reserva memoria para guardar la información del nuevo proceso.
       Contempla que puede producirse error en la reserva.
       Devuelve MEM_ERROR
    */

    nuevo->br999_pid = pid;

    /* Modifica el valor del pid de la estructura reservada
       dinámicamente accediendo a través del operador '->' */

    strncpy(nuevo->br999_ejecutable, ejecutable, MAX_EJECUTABLE);

    /* Utiliza la función strncpy para copiar el parámetro con la ruta del
    ejecutable a la estructura */

    nuevo->br999_estado = estado;
    /* establece su estado */
    procesos[ix] = nuevo;
    /* Asigna a la posición ix del array de punteros el nuevo registro */

    return 0;
}

/* función para listar */
void listar_procesos(struct proceso *procesos[])
{
    int i;
    for (i = 0; i < MAX_PROCESOS; i++)
    {
        if (procesos[i] == NULL)
        {
            printf("Posicion de tabla %d:Vacia", i);
        }
        else
        {
            printf("Posicion de tabla %d:%d %s %d\n", i,
                   procesos[i]->br999_pid,
                   procesos[i]->br999_ejecutable,
                   procesos[i]->br999_estado

            );
        }
    }

    // /**** PRÁCTICA ****/
    /* Recorre el array de procesos con for, escribiendo con printf los campos
       de cada proceso. Si una posición está vacía, imprimir "Vacía" */
}

/* función para liberar */
void liberar_procesos(struct proceso *procesos[])
{
    int i;
    /**** PRÁCTICA ****/
    /* Recorre el array de procesos con for liberando la memoria de cada
       proceso */

    for (i = 0; i < MAX_PROCESOS; i++)
    {
        if (procesos[i] != NULL)
        {
            free(procesos[i]);
            procesos[i] = NULL;
        }
    }
}
