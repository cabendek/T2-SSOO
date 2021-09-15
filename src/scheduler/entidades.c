#include "entidades.h"

int pid = 0;
Process* process_init(char* nombre, int fabrica, int tiempo_llegada, int* burst){
    Process* process = malloc(sizeof(Process));
    
    *process = (Process) {
        .pid = pid,
        .nombre = nombre,
        .estado = READY,
        .id_fabrica = fabrica,
        .tiempo_llegada = tiempo_llegada,
        .section = 0, //Seccion 0: no esta en nada; Seccion 1: "READY" desde "WAITING"; Seccion 2: "READY" desde "RUNNING"; Seccion 3: "READY" por 1era vez; Seccion 4: "WAITING"
        .array_burst = burst,
        .number_burst = 0,
        .quantum = 0,
        .A = 0,
        .B = 0,
        .actual_burst = 0,
        .siguiente = NULL
  };
  pid += 1;
  return process;
}

Queue* queue_init(){
  Queue* queue = malloc(sizeof(Queue));

  *queue = (Queue) {
    .largo = 0,
    .primer_proceso = NULL,
    .ultimo_proceso = NULL
  };
  return queue;
};

Queue_secciones* queue_secciones_init(){
  Queue_secciones* cola_secciones = malloc(sizeof(Queue_secciones));
  
  *cola_secciones = (Queue_secciones){
    .seccion = calloc(4, sizeof(Queue**))
  };
  return cola_secciones;
}

void insertar_proceso(Process* proceso, Queue* cola){
  // si es el primer elemento
  if (cola->largo == 0){
    cola->primer_proceso == proceso;
    cola->ultimo_proceso == proceso;
    cola->largo = 1;
  } else{
  // si no es el primer elemento
  Process* ultimo_proceso = cola->ultimo_proceso;
  ultimo_proceso->siguiente = proceso;
  cola->ultimo_proceso = proceso;
  cola->largo += 1;
  }
}

void quitar_proceso(Process* proceso, Queue* cola){
  // si es el proceso inicial
  if (cola->primer_proceso == proceso){
    Process* proceso_siguiente = cola->primer_proceso->siguiente;
    cola->primer_proceso = proceso_siguiente;
    cola->largo =- 1;
  } else if (cola->ultimo_proceso == proceso){
  // si es el proceso final
    // iterar cola hasta encontrar el pen ultimo y asignarle a su siguiente NULL
    Process* proceso_iterado = cola->primer_proceso;
    while (proceso_iterado->siguiente->siguiente != NULL){
      proceso_iterado = proceso_iterado->siguiente;
    }
    proceso_iterado->siguiente = NULL;
    cola->largo =- 1;
  } else {
  // si es otro proceso
    int pid = proceso->pid;
    Process* proceso_iterado = cola->primer_proceso;
    while(proceso_iterado->siguiente->pid != pid){
      proceso_iterado = proceso_iterado->siguiente;
    }
    Process* siguiente = proceso_iterado->siguiente->siguiente;
    proceso_iterado->siguiente = siguiente;
    cola->largo =- 1;
  }
}

void inicializar_proceso(Process* proceso, Queue* cola_inicial, Queue_secciones* cola_secciones){
  // la cola final siempre es la seccion 3 (que en el arreglo es 2)
  Queue* cola_final = cola_secciones->seccion[2];
  quitar_proceso(proceso, cola_inicial);
  insertar_proceso(proceso, cola_final);
}

void cambiar_seccion(Process* proceso, int seccion_inicial, int seccion_final, Queue_secciones* cola_secciones){
  Queue* cola_inicial = cola_secciones->seccion[seccion_inicial - 1];
  Queue* cola_final = cola_secciones->seccion[seccion_final - 1];
  quitar_proceso(proceso, cola_inicial);
  insertar_proceso(proceso, cola_final);
}

void finalizar_proceso(Process* proceso, int seccion_inicial, Queue* cola_final, Queue_secciones* cola_secciones){
  Queue* cola_inicial = cola_secciones->seccion[seccion_inicial - 1];
  quitar_proceso(proceso, cola_inicial);
  insertar_proceso(proceso, cola_final);
}

void destroy_proceso(Process* proceso){
  if (proceso->siguiente != NULL){
    destroy_proceso(proceso->siguiente);
  } else{
    free(proceso->siguiente);
    free(proceso);
  }
}

void destroy_queue(Queue* cola){
  destroy_proceso(cola->primer_proceso);
  free(cola);
}

void destroy_queue_secciones(Queue_secciones* cola){
  free(cola->seccion);
  free(cola);
}

// int quantum(int Q, int fabrica, Queue* queue){
//     Process* process;
//     int ni = 0;
//     int fabricas[] = {0,0,0,0};
//     process = queue->primero;
//     while (process != NULL){
//         if (process->id_fabrica == fabrica){
//             ni += 1;
//             fabricas[fabrica-1] = 1;
//         } else {
//             fabricas[process->id_fabrica-1] = 1;
//         }
//         process = process->siguiente;
//     }
    
//     int f = fabricas[0]+fabricas[1]+fabricas[2]+fabricas[3];
//     int q = Q/(ni*f);
//     return q;
// }

// // Armar función que cambie el estado del proceso
// if (Ai == tiempo_que_lleva){
//   //Cede la CPU
//   process->estado = WAITING;
// } else if (Termina la ejecucion){
//   process->estado = FINISHED;
// } else if (quantum_restante == 0){
//   process->estado = READY;
// } else {
//   //Sigo en running
// }

// //FUNCION
// switch (process->estado) //Switch te permite realizar diferentes acciones según el caso del estado
// {
// case RUNNING:
//   printf("Estoy en estado RUNNING\n");
//   break;

// case READY:
//   printf("Estoy en estado READY\n");
//   printf("Lo mando al final de la cola\n");
//   break;

// case WAITING:
//   printf("Estoy en estado WAITING\n");
//   printf("Lo mando al final de la cola\n");
//   break;

// case FINISHED:
//   printf("Estoy en estado FINISHED\n");
//   break;

// default:
// printf("Esto no debería pasar\n");
//   break;
// }

