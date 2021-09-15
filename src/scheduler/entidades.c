#include <entidades.h>

Process* process_init(int pid, char* nombre, int fabrica, int tiempo_llegada, int* burst){
    Process* process = malloc(sizeof(Process));
    
    *process = (Process) {
        .pid = pid,
        .nombre = nombre,
        .estado = READY,
        .id_fabrica = fabrica,
        .tiempo_llegada = tiempo_llegada,
        .cant_rafagas = 0,
        .siguiente = NULL,
        .burst = burst
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