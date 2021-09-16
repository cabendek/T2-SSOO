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


int main(int argc, char **argv)
{
  int Q = 100;
  printf("Hello T2!\n");

  // inicializamos las variables leyendo el archivo
  InputFile *file = read_file("input.txt");
  Queue* cola_inicial = queue_init();
  Queue* cola_seccion1 = queue_init();
  Queue* cola_seccion2 = queue_init();
  Queue* cola_seccion3 = queue_init();
  Queue* cola_seccion4 = queue_init();
  Queue* cola_final = queue_init();
  Queue_secciones* cola_secciones = queue_secciones_init(cola_seccion1, cola_seccion2, cola_seccion3, cola_seccion4);

  int total_length = file->len;
  // printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
    // printf(
    //     "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
    //     line[0], line[2], line[1], line[3]);
        int largo_array = atoi(line[3])*2-1;
        int array_burst[largo_array];
        for (int i=0; i< largo_array; i++){
          array_burst[i]= atoi(line[4+i]);
          printf("Array_burst[%i] = %i\n", i, array_burst[i]);
        }
        char* nombre = line[0];
        Process* proceso = process_init(nombre, atoi(line[2]), atoi(line[1]), array_burst, atoi(line[3]));
        if (proceso->tiempo_llegada == 0){
          printf("[t = 0] El proceso %s ha sido creado y ha pasado a estado READY.\n", proceso->nombre);
          printf("A inicial = %i\n", proceso->A);
          insertar_proceso(proceso, cola_seccion3);
          proceso->section = 3;
        } else {
          insertar_proceso(proceso, cola_inicial);
          printf("[t = 0] El proceso %s ha sido creado.\n", proceso->nombre);
        }
  }
  // printf("Liberando memoria...\n");
  printf("Primer proceso: %s\n ", cola_seccion3->primer_proceso->nombre);



  // Instanciar Plani <3

int time = 0;
// while (cola_final->largo != total_length){
for (int t = 0; t < 6; t++){
    bool process_running = false;
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
        //running_process->B = running_process->array_burst[running_process->actual_burst];
        printf("B: %i\n",running_process->B);
        printf("Actual Burst: %i\n",running_process->actual_burst);
        cambiar_seccion(running_process, running_process->section, 4, cola_secciones, time);
        if (running_process->quantum == 0){
          //running_process.interrupciones += 1; ESTADISTICAS
        }
  //       // Buscar cual entra --------------- FUNCION -----------------
  //       // READY -> RUNNING
  //       Process* new_running = buscar_proceso_running(cola_secciones);
  //       if (new_running != NULL){
  //         int q = quantum(Q,new_running->id_fabrica,cola_secciones);
  //         new_running->quantum = q;
  //         new_running->A -= 1;
  //         new_running->quantum -= 1;
  //         printf("[t = %i] La CPU eligió el proceso %s.\n", time, new_running->nombre);
        // }

      } else if (running_process->A == 0 && running_process->number_burst == (running_process->actual_burst+2)/2){
  //       // RUNNING -> FINISHED
  //       running_process->estado = FINISHED;
  //       finalizar_proceso(running_process, running_process->section, cola_final, cola_secciones, time);
  //       // Buscar cual entra --------------- FUNCION -----------------
  //       // READY -> RUNNING
  //       Process* new_running = buscar_proceso_running(cola_secciones);
  //       if (new_running != NULL){
  //         printf("[t = %i] La CPU eligió el proceso %s.\n", time, new_running->nombre);
  //         int q = quantum(Q,new_running->id_fabrica,cola_secciones);
  //         new_running->quantum = q;
  //         new_running->A -= 1;
  //         new_running->quantum -= 1;
        // }

      } else if (running_process->quantum == 0){
        printf("El proceso %s pasa a READY\n", running_process->nombre);
        // RUNNING -> READY
        running_process->estado = READY;
        cambiar_seccion(running_process,running_process->section, 2, cola_secciones, time);
        // //Buscar cual entra --------------- FUNCION -----------------
        // // READY -> RUNNING
        Process* new_running = buscar_proceso_running(cola_secciones);
        printf("El proceso %s pasa a RUNNING\n", running_process->nombre);
        if (new_running != NULL){
          printf("[t = %i] La CPU eligió el proceso %i.\n", time, new_running->pid);
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
        printf("[t = %i] Proceso %s corriendo en la CPU.\n", time, running_process->nombre);
        printf("Me queda A = %i.\n", running_process->A);
        printf("Me queda Quantum = %i.\n", running_process->quantum);
      }
    
  //   // Si no hay procesos en estado RUNNING
    } else {
      // READY -> RUNNING
      bool ningun_proceso = true;
      for (int i=0; i<3;i++){
        if (cola_secciones->seccion[i]->largo > 0 ){
          running_process = cola_secciones->seccion[i]->primer_proceso;
          running_process->estado = RUNNING;
          printf("[t = %i] La CPU eligió el proceso %s y lo ejecutó 1 segundo.\n", time, running_process->nombre);
          int q = quantum(Q, running_process->id_fabrica,cola_secciones);
          running_process->quantum = q;
          running_process->A -= 1;
          running_process->quantum -= 1;
          ningun_proceso = false;
          printf("Me queda A = %i.\n", running_process->A);
          printf("Me queda Quantum = %i.\n", running_process->quantum);
          break;
        }
      }
      if (ningun_proceso){
        printf("[t = %i] No hay ningun proceso ejecutando en la CPU.\n",time);
      }
    }

  //   // Descontar B de procesos waiting
  //   Process* proceso_waiting = cola_seccion4->primer_proceso;

  //   while (proceso_waiting != NULL){
  //     proceso_waiting->B -= 1;

  //     if (proceso_waiting->B == 0){
  //       // WAITING -> READY
  //       cambiar_seccion(proceso_waiting, 4, 1, cola_secciones, time);
  //       proceso_waiting->actual_burst += 1;
  //       proceso_waiting->A = proceso_waiting->array_burst[proceso_waiting->actual_burst];
  //     }
      
  //     proceso_waiting = proceso_waiting->siguiente;
  //   }
    

  //   // CHEQUEO LA COLA INICAL         INICIAL -> READY
  //   if (cola_inicial->largo == 1){
  //     if (cola_inicial->primer_proceso->tiempo_llegada == time){
  //       insertar_proceso(cola_inicial->primer_proceso, cola_seccion3);
  //     }
    
  //   } else {
  //     Process* process_1 = NULL;
  //     Process* process_2 = NULL;
  //     Process* proceso_sin_iniciar = cola_inicial->primer_proceso;

  //     while(proceso_sin_iniciar != NULL){
  //       if (proceso_sin_iniciar->tiempo_llegada == time){
          
  //         if (process_1 == NULL){
  //           process_1 = proceso_sin_iniciar;

  //         } else if (process_2 == NULL){
  //           process_2 = proceso_sin_iniciar;

  //         } else {
  //           printf("Esto no deberia pasar\n");
  //         }
  //       }
  //       proceso_sin_iniciar = proceso_sin_iniciar->siguiente;
  //     }
  //     if (process_2 == NULL && process_1 != NULL){
  //       inicializar_proceso(process_1, cola_inicial, cola_secciones, time);
  //     } else if (process_2 != NULL && process_1 != NULL){
  //       int ingreso = prioridad(process_1, process_2);
  //       if (ingreso == 0){
  //         inicializar_proceso(process_1, cola_inicial, cola_secciones, time);
  //         inicializar_proceso(process_2, cola_inicial, cola_secciones, time);
  //       } else{
  //         inicializar_proceso(process_2, cola_inicial, cola_secciones, time);
  //         inicializar_proceso(process_1, cola_inicial, cola_secciones, time);
  //       }
  //     } else {
  //       // No se incorpora ningun proceso
  //     }
    // }

  time += 1;
  }
  printf("\tTERMINEEE\n");
  input_file_destroy(file);
  destroy_queue(cola_inicial);
  destroy_queue(cola_seccion1);
  destroy_queue(cola_seccion2);
  destroy_queue(cola_seccion3);
  destroy_queue(cola_seccion4);
  destroy_queue(cola_final);
  destroy_queue_secciones(cola_secciones);
}
