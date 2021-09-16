#include "entidades.h"
#include <string.h>

int pid = 0;
Process* process_init(char* nombre, int fabrica, int tiempo_llegada, int* array_burst, int number_burst){
    
    Process* process = malloc(sizeof(Process));
    
    *process = (Process) {
        .pid = pid,
        .nombre = nombre,
        .estado = READY,
        .id_fabrica = fabrica,
        .tiempo_llegada = tiempo_llegada,
        .section = 0, //Seccion 0: no esta en nada; Seccion 1: "READY" desde "WAITING"; Seccion 2: "READY" desde "RUNNING"; Seccion 3: "READY" por 1era vez; Seccion 4: "WAITING"
        .array_burst = array_burst,
        .number_burst = number_burst,
        .quantum = 0,
        .A = array_burst[0],
        .B = array_burst[1],
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
    cola->primer_proceso = proceso;
    cola->ultimo_proceso = proceso;
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
    cola->ultimo_proceso = proceso_iterado;
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
  if (cola->largo == 0){
    cola->primer_proceso = NULL;
    cola->ultimo_proceso = NULL;
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
  proceso->section = seccion_final;
  quitar_proceso(proceso, cola_inicial);
  insertar_proceso(proceso, cola_final);
}

void finalizar_proceso(Process* proceso, int seccion_inicial, Queue* cola_final, Queue_secciones* cola_secciones){
  Queue* cola_inicial = cola_secciones->seccion[seccion_inicial - 1];
  quitar_proceso(proceso, cola_inicial);
  insertar_proceso(proceso, cola_final);
}

Process* buscar_proceso_running(Queue_secciones* cola_secciones){
  for (int i=0; i<3; i++){
    if (cola_secciones->seccion[i]->largo > 0){
      return cola_secciones->seccion[i]->primer_proceso;
    }
  }
  return NULL;
}

Process* prioridad(Process* process_1, Process* process_2){
  if (process_1->id_fabrica == process_2->id_fabrica){
    int result = strcmp(process_1->nombre,process_2->nombre);
    if (result > 0) {
      // Process 2 es mas chico
      return process_2;
    
    } else {
      // Process 1 es mas chico
      return process_1;
    }
  } else{
    if (process_1->id_fabrica < process_2->id_fabrica){
      return process_1;
    } else {
      return process_2;
    }
  }
}

void destroy_proceso(Process* proceso){
  if (proceso->siguiente != NULL){
    destroy_proceso(proceso->siguiente);
  } else{
    // free(proceso->array_burst);
    free(proceso);
  }
}

void destroy_queue(Queue* cola){
  if (cola->primer_proceso != NULL){
    destroy_proceso(cola->primer_proceso);
  }
  free(cola);
}

void destroy_queue_secciones(Queue_secciones* cola){
  free(cola->seccion);
  free(cola);
}

// int quantum(int Q, int fabrica, Queue* queue){
  
  // int fabricas_1[]= {0,0,0,0};
  //   int fabricas_2[]= {0,0,0,0};

  //   for (int j=0;j<4;j++){
  //     Process* actual = cola_secciones->seccion[j]->primer_proceso;
  //     while (actual != NULL)
  //     {
  //       if (actual->id_fabrica == process_1->id_fabrica)
  //       {
  //         /* code */
  //       }
  //     }
  //   }


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

