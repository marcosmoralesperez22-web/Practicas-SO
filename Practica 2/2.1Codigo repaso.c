// un poco de codigo de repaso para C

int Marcos = 23; // variable global inicializada
int Mario;       // variable global no inicializada

// variable global porque se encuentra fuera de cualquier funcion
// inicializada cuando se le da un valor

/*Declaracion del tipo que representa un DNI*/




struct dni
{
    int numero; // numero correspondiente al DNI y variable no global
    char letra; // letra correspondiente al DNI
};

#define NUM_LETRAS 23 /* directiva de preprocesador                              \
-no ocupa memoria en ejecucion                                                   \
-le dice al compilador que "NUM LETRAS" le ponga un 23                           \
 -Basicamente es una constante                                                   \
 -Ese 23 representa el alfabeto español sin la ñ (es algo que no va a cambiar) \
*/

const char letra_correcta[NUM_LETRAS] = // esto es un array de tamaño NUM_LETRAS
                                        // const significa que no podemos cambiar el tamaño del array
    {
        'A', 'G', 'M', 'Y', 'F', 'P', 'D', 'X',
        'B', 'N', 'J', 'Z', 'S', 'Q', 'V', 'H', 'L', 'C', 'K', 'E'};

// ejemplo DNi

const struct dni ejemplo = {12345678, 'Z'};

// comprobar si un dni es valido

if (ejemplo.letra == letra_correcta[ejemplo.número % NUM_LETRAS])
{
    // esta comprobacion es la formula del dni que se queda con el resto y lo asigna a una posicion a cada letra
    // solo se comprueba si la posicion coincide con la letra del dni
    printf("correcto\n");
}
else
{
    printf("incorrecto\n")
}

// \n para salto de linea


//para saber la cantidad de bytes necesarios para representar un elemento de un determinado tipo se usa syzeof


//si char=1
//si float = 4
sizeof(char)
sizeof(long long) seria 8