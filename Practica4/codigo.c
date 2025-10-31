




/* 
 * Esta función sirve para decidir:
 * - Cuánto tiempo de CPU (quantum) se le asigna al proceso para su próxima ejecución.
 * - La prioridad con la que debe entrar en la cola de planificación.
 * - Si debe encolarse por delante o por detrás (front o back).
 */

PRIVATE void sched(rp, queue, front)


register struct proc *rp; /* process to be scheduled */
int *queue; /* return: queue to use */
int *front; /* return: front or back */
{

// time_left indica si el proceso aún tiene tiempo de quantum restante
 int time_left = (rp->p_ticks_left > 0); /* cuanto consumido completamente si no es mayor a 0 */
 /* Si el proceso ha agotado su quantum (time_left == 0):
 - Se le asigna un nuevo quantum igual a p_quantum_size.already is in the
 * - Se baja su prioridad (aumenta el valor numérico) a menos que ya esté en la cola de menor prioridad.

 Si está activada la política plotsched y el proceso es "doCPU":
     * - Se asigna una prioridad aleatoria entre 5 y 14.
     */


 
if (!time_left) { 
    //reinicia el cuanto del proceso para la proxima ejecucion
    rp->p_ticks_left = rp->p_quantum_size;
    
    
    if (plotsched == 1 && strncmp(rp->p_name, "doCPU", 5) == 0) {
        //strnmcmp comprueba si los 5 primeros caracteres son iguales a "doCPU"
        //para procesos doCPU prioridad aleatoria entre 5 y 14
        rp->p_priority = 5 + rand() % 10; //aqui es numero random de 0 a 9 y + 5 =14
    } else {
        //para cualquier otro proceso se incrementa la prioridad
        if (rp->p_priority < IDLEQ - 1) rp->p_priority++;
}
//IDLEQ es una constante que indica el valor de la cola de menor prioridad en el sistema, normalmente el proceso inactivo (idle).

//La condición rp->p_priority < IDLEQ - 1 comprueba que la prioridad del proceso sea menor que el valor inmediatamente anterior a IDLEQ. Esto evita que la prioridad se incremente más allá del límite máximo permitido.

//la cola a la que se asigna es la basada en la nueva prioridad calculada
*queue = rp->p_priority;


//la insercion será por delante (front=1) si queda tiempo en el cuanto, o por detras si front=0 si no queda tiempo
 *front = time_left;
}


}





PRIVATE void enqueue(rp)

register struct proc *rp; 

{
 int q; /* cola de planificacion a usar*/
 int front; /* indica si se añade al principio o al final */


 #if DEBUG_SCHED_CHECK
 //verifica el estado de las colas de planificacion antes de hacer cambios
 check_runqueues("enqueue");
 //alerta si el proceso ya esta marcado como listo
 if (rp->p_ready) kprintf("enqueue() already ready process\n");
#endif
//registra la accion de encolar el proceso para la visualizacion o depuracion
 plotear(rp, opENQUEUE) ; 
 /* llama a la funcion sched para decidir en que cola se debe insertar el proceso y si debe ir al principio o al final */
 sched(rp, &q, &front);
 //registra la decision tomada por el planificador para depuracion o graficos
 plotear(rp, (q << 16) | (front << 8) | opSCHED) ; 

//si la politica elegida (plotsched) es 2 inserta el proceso ordenadamente
if (plotsched == 2) {
    struct proc *prev = NULL; //proceso anterior a la posicion de insercion
     struct proc *curr = rdy_head[q]; //proceso actual en la cola 'q' de donde empezamos a buscar

    while (curr != NIL_PROC && curr->p_nr > rp->p_nr) {
        prev = curr; //guarda el proceso actual como previo
        curr = curr->p_nextready; // avanza al siguiente proceso en la lista
    }
    //si no hay proceso anterior (es decir, rp será el primero de la cola)
    if (prev == NULL) {
        rp->p_nextready = rdy_head[q]; //el nuevo proceso apunta al inicio actual
        rdy_head[q] = rp; //rp pasa a ser el nuevo inicio
        if (rdy_tail[q] == NIL_PROC) {
            rdy_tail[q] = rp;
        }
    } else { 
        //inserta el proceso entre prev y curr
        rp->p_nextready = curr; //rp apunta al proceso actual
        prev->p_nextready = rp; //el proceso previo apunta a rp
        //si rp es el ultimo (curr es NULL) actualiza el puntero tail de la cola 
        if (curr == NIL_PROC) {
            rdy_tail[q] = rp;
        }
    }
            //asegura que el ultimo proceso en la cola apunta a NULL 
    if (rdy_tail[q] != NIL_PROC) {
        rdy_tail[q]->p_nextready = NIL_PROC;
    }
}

//selecciona el proximo proceso a ejecutar (cambio de contexto)
 pick_proc();

#if DEBUG_SCHED_CHECK

 rp->p_ready = 1; //marca que el proceso esta listo para ejecutarse
 check_runqueues("enqueue"); //verifica el estado de las colas tras encolar
#endif
}
