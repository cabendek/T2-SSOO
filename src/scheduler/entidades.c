#include <entidades.h>

Process* process_init(int pid, char* nombre, int fabrica, int tiempo_llegada, int* burst, int number_burst){
    Process* process = malloc(sizeof(Process));
    
    *process = (Process) {
        .pid = pid,
        .nombre = nombre,
        .estado = READY,
        .id_fabrica = fabrica,
        .tiempo_llegada = tiempo_llegada,
        .section = 0, //Seccion 0: no esta en nada; Seccion 1: "READY" desde "WAITING"; Seccion 2: "READY" desde "RUNNING"; Seccion 3: "READY" por 1era vez; Seccion 4: "WAITING"
        .array_burst = burst,
        .number_burst = number_burst,
        .quantum = 0,
        .A = 0,
        .B = 0,
        .actual_burst = 0,
        .siguiente = NULL
  };
  return process;
}

Queue* queue_init(){
  Queue* queue = malloc(sizeof(Queue));

  *queue = (Queue) {
    .length = 0,
    .primero = NULL,
    .ultimo = NULL
  };
  return queue;
};

int quantum(int Q, int fabrica, Queue* queue){
    Process* process;
    int ni = 0;
    int fabricas[] = {0,0,0,0};
    process = queue->primero;
    while (process != NULL){
        if (process->id_fabrica == fabrica){
            ni += 1;
            fabricas[fabrica-1] = 1;
        } else {
            fabricas[process->id_fabrica-1] = 1;
        }
        process = process->siguiente;
    }
    
    int f = fabricas[0]+fabricas[1]+fabricas[2]+fabricas[3];
    int q = Q/(ni*f);
    return q;
};


// Armar función que cambie el estado del proceso
if (Ai == tiempo_que_lleva){
  //Cede la CPU
  process->estado = WAITING;
} else if (Termina la ejecucion){
  process->estado = FINISHED;
} else if (quantum_restante == 0){
  process->estado = READY;
} else {
  //Sigo en running
}


//FUNCION
switch (process->estado) //Switch te permite realizar diferentes acciones según el caso del estado
{
case RUNNING:
  printf("Estoy en estado RUNNING\n");
  break;

case READY:
  printf("Estoy en estado READY\n");
  printf("Lo mando al final de la cola\n");
  break;

case WAITING:
  printf("Estoy en estado WAITING\n");
  printf("Lo mando al final de la cola\n");
  break;

case FINISHED:
  printf("Estoy en estado FINISHED\n");
  break;

default:
printf("Esto no debería pasar\n");
  break;
}

