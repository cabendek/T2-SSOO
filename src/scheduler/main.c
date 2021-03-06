#include <stdio.h>
#include <stdlib.h>
#include "entidades.h"
#include "../file_manager/manager.h"

#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// FALTA
// 1) Generar las estadisticas 
//    cantidad elecciones CPU (listo)
//    cantidad interrupciones (listo)
//    turnaround time (listo)
//    response time (listo)
//    waiting time (FALTA Checkear)
// 2) Usando las estadisticas, generar el output_file (Y LIBERARLO) (LISTO)
// 3) Recibir un Q diferente de 0
// 4) CAMBIAR EL de 4 a q EN EL RETURN DE LA FUNCION QUANTUM (LISTO)

int main(int argc, char **argv)
{
  int Q = 100;
  printf("Hello T2!\n");

  // inicializamos las variables leyendo el archivo
  InputFile *file = read_file(argv[1]);
  FILE *output_file = fopen(argv[2], "w");
  if (argv[3] == NULL){
    printf("NO HAY VALOR DE Q\n");
  } else {
    Q = atoi(argv[3]);
  }
  Queue* cola_inicial = queue_init();
  Queue* cola_seccion1 = queue_init();
  Queue* cola_seccion2 = queue_init();
  Queue* cola_seccion3 = queue_init();
  Queue* cola_seccion4 = queue_init();
  Queue* cola_final = queue_init();
  Queue_secciones* cola_secciones = queue_secciones_init(cola_seccion1, cola_seccion2, cola_seccion3, cola_seccion4);

  int total_length = file->len;
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
        char* nombre = line[0];
        Process* proceso = process_init(nombre, atoi(line[2]), atoi(line[1]), atoi(line[3]));
        int largo_array = atoi(line[3])*2-1;
        for (int i=0; i< largo_array; i++){
          proceso->array_burst[i]=atoi(line[3+i+1]);
        }
        proceso->A = proceso->array_burst[0];
        
        if (proceso->tiempo_llegada == 0){
          printf("[t = 0] El proceso %s ha sido creado.\n", proceso->nombre);
          insertar_proceso(proceso, cola_seccion3);
          proceso->section = 3;
          proceso->estado = READY;
        } else {
          insertar_proceso(proceso, cola_inicial);
          printf("[t = 0] El proceso %s ha sido creado.\n", proceso->nombre);
        }
  }

 // Comienza a ejecutar nuestra schedule: Plani <3

int time = 0;
while (cola_final->largo != total_length){
  printf("--------------------------------------\n");
    bool process_running = false;
    bool ningun_proceso;
    Process* running_process;

    for (int i=0; i<3;i++){
      if (cola_secciones->seccion[i]->largo > 0 ){
        if (cola_secciones->seccion[i]->primer_proceso->estado == RUNNING){ //Esto es pensando que solo el primer proceso puede estar en running
          process_running = true;
          running_process = cola_secciones->seccion[i]->primer_proceso;
          break;
        }
      }
    }
    
    if (process_running){

      if (running_process->A == 0 && running_process->number_burst != (running_process->actual_burst+2)/2) {
        // RUNNING -> WAITING
        running_process->estado = WAITING;
        running_process->actual_burst += 1;
        running_process->B = running_process->array_burst[running_process->actual_burst]+1;

        cambiar_seccion(running_process, running_process->section, 4, cola_secciones, time);
        if (running_process->quantum == 0){
          printf("//// ENTRE AQUI ////\n");
          running_process->cantidad_interrupciones += 1;
        }
  //       // Buscar cual entra --------------- FUNCION ----------------- (con mas de un proceso)
  //       // READY -> RUNNING
        Process* new_running = buscar_proceso_running(cola_secciones);
        if (new_running != NULL){
           new_running->estado = RUNNING;
          int q = quantum(Q,new_running->id_fabrica,cola_secciones);
          new_running->quantum = q;
          new_running->A -= 1;
          new_running->quantum -= 1;
          new_running->cantidad_elecciones_CPU += 1;
          printf("[t = %i] La CPU eligio el proceso %s y lo corrio 1 segundo.\n", time, new_running->nombre);
          if (new_running->tiempo_primera_atencion == -1){
            new_running->tiempo_primera_atencion = time;
          }
        }

      } else if (running_process->A == 0 && running_process->number_burst == (running_process->actual_burst+2)/2){
        // RUNNING -> FINISHED
        running_process->estado = FINISHED;
        running_process->tiempo_finalizacion = time;
        finalizar_proceso(running_process, running_process->section, cola_final, cola_secciones, time);
        if (running_process->quantum == 0){
          running_process->cantidad_interrupciones += 1;
        }
        // Buscar cual entra --------------- FUNCION -----------------
        // READY -> RUNNING
        Process* new_running = buscar_proceso_running(cola_secciones);
        if (new_running != NULL){
          new_running->cantidad_elecciones_CPU += 1;
          printf("[t = %i] La CPU eligio el proceso %s y lo corrio 1 segundo.\n", time, new_running->nombre);
          if (new_running->tiempo_primera_atencion == -1){
            new_running->tiempo_primera_atencion = time;
          }
          int q = quantum(Q,new_running->id_fabrica,cola_secciones);
          new_running->quantum = q;
          new_running->A -= 1;
          new_running->quantum -= 1;
        }

      } else if (running_process->quantum == 0){
        // RUNNING -> READY
        running_process->estado = READY;
        printf("//// ENTRE AQUI ////\n");
        running_process->cantidad_interrupciones += 1;
        cambiar_seccion(running_process,running_process->section, 2, cola_secciones, time);
        //Buscar cual entra --------------- FUNCION -----------------
        // READY -> RUNNING
        Process* new_running = buscar_proceso_running(cola_secciones);
        if (new_running != NULL){
          new_running->cantidad_elecciones_CPU += 1;
          printf("[t = %i] La CPU eligio el proceso %s y lo corrio 1 segundo.\n", time, new_running->nombre);
          if (new_running->tiempo_primera_atencion == -1){
            new_running->tiempo_primera_atencion = time;
          }
          int q = quantum(Q,new_running->id_fabrica,cola_secciones);
          new_running->estado = RUNNING;
          new_running->quantum = q;
          new_running->A -= 1;
          new_running->quantum -= 1;
        }
        
      } else {
  //       // RUNNING -> RUNNING
        running_process->A -= 1;
        running_process->quantum -= 1;
        printf("[t = %i] Proceso %s corriendo en la CPU 1 segundo.\n", time, running_process->nombre);
      }
    
  //   // Si no hay procesos en estado RUNNING
    } else {
      // READY -> RUNNING
      ningun_proceso = true;
      for (int i=0; i<3;i++){
        if (cola_secciones->seccion[i]->largo > 0 ){
          running_process = cola_secciones->seccion[i]->primer_proceso;
          running_process->estado = RUNNING;
          running_process->cantidad_elecciones_CPU += 1;
          printf("[t = %i] La CPU eligio el proceso %s y lo ejecut?? 1 segundo.\n", time, running_process->nombre);
          if (running_process->tiempo_primera_atencion == -1){
            running_process->tiempo_primera_atencion = time;
          }
          int q = quantum(Q, running_process->id_fabrica,cola_secciones);
          running_process->quantum = q;
          running_process->A -= 1;
          running_process->quantum -= 1;
          ningun_proceso = false;
          break;
        }
      }
      if (ningun_proceso){
        printf("[t = %i] No hay ningun proceso ejecutando en la CPU.\n",time);
      }
    }

    // Descontar B de procesos waiting
    Process* proceso_waiting = cola_seccion4->primer_proceso;
    while (proceso_waiting != NULL){
      proceso_waiting->B -= 1;
      proceso_waiting->waiting += 1;
      if (proceso_waiting->B == 0){
        // WAITING -> READY
        cambiar_seccion(proceso_waiting, 4, 1, cola_secciones, time);
        proceso_waiting->actual_burst += 1;
        proceso_waiting->A = proceso_waiting->array_burst[proceso_waiting->actual_burst];
        if (ningun_proceso){
          // READY -> RUNNING
          Process* new_running = buscar_proceso_running(cola_secciones);
          if (new_running != NULL){
            new_running->cantidad_elecciones_CPU += 1;
            printf("[t = %i] La CPU eligio el proceso %s y lo corrio 1 segundo.\n", time, new_running->nombre);
            if (new_running->tiempo_primera_atencion == -1){
              new_running->tiempo_primera_atencion = time;
            }
            int q = quantum(Q,new_running->id_fabrica,cola_secciones);
            new_running->estado = RUNNING;
            new_running->quantum = q;
            new_running->A -= 1;
            new_running->quantum -= 1;
          }
        }
      }
      
      proceso_waiting = proceso_waiting->siguiente;
    }

    for (int i=0; i<3;i++){
      Process* actual = cola_secciones->seccion[i]->primer_proceso;
      while (actual != NULL) {
        if (actual->estado == READY){
          actual->waiting += 1;
        }
        actual = actual->siguiente;
      }
    }
    
    time += 1;

    // CHEQUEO LA COLA INICAL         INICIAL -> READY
    if (cola_inicial->largo == 1){
      if (cola_inicial->primer_proceso->tiempo_llegada == time){
        inicializar_proceso(cola_inicial->primer_proceso, cola_inicial,cola_secciones, time);
      }
    
    } else {
      Process* process_1 = NULL;
      Process* process_2 = NULL;
      Process* proceso_sin_iniciar = cola_inicial->primer_proceso;

      while(proceso_sin_iniciar != NULL){
        if (proceso_sin_iniciar->tiempo_llegada == time){
          
          if (process_1 == NULL){
            process_1 = proceso_sin_iniciar;

          } else if (process_2 == NULL){
            process_2 = proceso_sin_iniciar;

          } else {
            printf("Esto no deberia pasar\n");
          }
        }
        proceso_sin_iniciar = proceso_sin_iniciar->siguiente;
      }
      if (process_2 == NULL && process_1 != NULL){
        inicializar_proceso(process_1, cola_inicial, cola_secciones, time);
      } else if (process_2 != NULL && process_1 != NULL){
        int ingreso = prioridad(process_1, process_2);
        if (ingreso == 0){
          inicializar_proceso(process_1, cola_inicial, cola_secciones, time);
          inicializar_proceso(process_2, cola_inicial, cola_secciones, time);
        } else{
          inicializar_proceso(process_2, cola_inicial, cola_secciones, time);
          inicializar_proceso(process_1, cola_inicial, cola_secciones, time);
        }
      } else {
        // No se incorpora ningun proceso
      }
    }
  }
  // crear output
  //Ordenarlo antes de...
  for (int i=0;i<total_length;i++){
    Process* proceso = cola_final->primer_proceso;
    while (proceso != NULL){
      if (proceso->pid == i){
        int turnaround_time = proceso->tiempo_finalizacion - proceso->tiempo_llegada;
        int response_time = proceso->tiempo_primera_atencion - proceso->tiempo_llegada;
        fprintf(output_file,"%s,", proceso->nombre);
        fprintf(output_file,"%i,", proceso->cantidad_elecciones_CPU);
        fprintf(output_file,"%i,", proceso->cantidad_interrupciones);
        fprintf(output_file,"%i,", turnaround_time);
        fprintf(output_file,"%i,", response_time);
        fprintf(output_file,"%i\n", proceso->waiting);
        break;
      }
      proceso = proceso->siguiente;
    }
  }

  destroy_queue(cola_inicial);
  destroy_queue(cola_seccion1);
  destroy_queue(cola_seccion2);
  destroy_queue(cola_seccion3);
  destroy_queue(cola_seccion4);
  destroy_queue(cola_final);
  destroy_queue_secciones(cola_secciones);
  input_file_destroy(file);
  fclose(output_file);
}
