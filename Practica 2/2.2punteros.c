// los punteros sirven para trabajar con direcciones de memoria

int *ptr; // esto es un puntero a int

// para obtener la direccion se usa &

// se puede entender como (int*) ptr, ptr es una variable de tipo int*

// o se puede entender como int (*ptr)

int num;
int *ptr;
num = 10;
ptr = &num; // ptr esta almacenando la direccion de memoria donde se encuentra num

/*por ejemplo en nuestro dni :
const struct dni ejemplo = {12345678, ‘Z’};
struct dni * const ptr = &ejemplo;

//para acceder podriamos usar (*ptr).numero pero se va a usar ptr->numero
*/

/* punteros.c */

#include <stdio.h>
/* Función que modifica el valor de una variable recibida por referencia */
void modificar_valor(int *ptr) // se le pasa un puntero
{
    /* Cambia el valor de la variable a la que apunta el puntero */
    *ptr = 20;
}
int main(int argc, char *argv[])
{
    int numero;
    numero = 10;
    printf("Antes de la función: %d\n", numero);
    /* Llamada a la función pasando la dirección de la variable */
    modificar_valor(&numero);
    printf("Después de la función: %d\n", numero);
    return 0;
}

// 10
// 20

#define NUM_FUNCIONES 4
static void (*const ejecutar[NUM_FUNCIONES])(const struct orden *orden) =
    {
        ejecutar_pwd,
        ejecutar_cd,
        ejecutar_jobs,
        ejecutar_orden_externa};

// static es que solo se puede usar en este archivo donde se define
// void no devuelve nada
//  (* const ejecutar[NUM_FUNCIONES]):
/*
ejecutar es un array de 4 punteros a funciones
cada puntero apunta a una funcion que no devueve nada
cada funcion recibe como parametro un puntero a una structura orden
el const indica que los punteros no pueden cambiar pero las funciones si.

//ejecutar[0]; ejecutaria pwd
//ejecutar[1]; ejecutaria cd

*/

struct orden actual;

int cadena2(void)
{
    const char cadena[] = "HOLA"; /*Array de 5 char (caracteres): 'h', 'o','l','a',\0'*/
    printf("%s\n", cadena);       //%s es el formato para imprimir char o cadenas en c

    return 0;
}

// en c las cadenas terminan en caracter nulo \0 asi que hay que contar un caracter mas

// verifica si la cadena ingresada supera el limite de caracteres (MAX_LEN) sin incluir el salto de linea \n
// si es demasiado larga limpia el buffer para evitar problemas en futuras lecturas
#define MAX_LEN 20 // ya sabemos lo que hace... declara constante
static int demasiado_larga(const char cadena[MAX_LEN])
{
    const int lon = strnlen(cadena, MAX_LEN);                    // esto lo que hace es calcular la longitud de la cadena hasta un maximo de MAX_LEN y evita leer mas alla del limite si no hay \0
    const int larga = !feof(stdin) && (cadena[lon - 1] != '\n'); // verifica si la cadena no terminó con un salto de linea \n. Si no terminó significa que el usuario escribió mas de MAX_LEN -1 caracteres. feof(stdin) comprueba si se llego al final de la entrada estandar
    if (larga)
    {
        fprintf(stderr, "respuesta demasiado larga\n"); // muestra mensaje en la salida de errores(stderr)
        /* descartar caracteres no leídos */
        // fprintf es igual que printf pero te deja elegir por donde quieres que sea el destino del mensaje
        while (!feof(stdin) && fgetc(stdin) != '\n')

        // feof comprueba si se ha llegado al final del archivo en la entrada estandar.
        // fgetc(stdin) lee un caracter de la entrada estandar. se comprueba con el salto de linea para saber si tiene que seguir leyendo.
        {
            /* bucle vacío */
        }
    }
    return larga;
}
static void leer_lin(const char pregunta[], char respuesta[MAX_LEN])
{
    char *err;
    do
    {
        fprintf(stdout, "%s ", pregunta);       /* mostrar la pregunta */
        err = fgets(respuesta, MAX_LEN, stdin); /* leer la respuesta */
    }
    // fgets es una función que lee una línea de texto desde una fuente (como el teclado o un archivo) y la guarda en un array de caracteres.