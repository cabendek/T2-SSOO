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
  printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
    printf(
        "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
        line[0], line[2], line[1], line[3]);
        int array_burst[atoi(line[3])];
        for (int i=0; i< atoi(line[3]); i++){
          array_burst[i]= atoi(line[3+i]);
        }
        Process* proceso = process_init(line[0], atoi(line[2]), atoi(line[1]), array_burst, atoi(line[3]));
        if (proceso->tiempo_llegada == 0){
          insertar_proceso(proceso, cola_seccion3);
        } else {
          insertar_proceso(proceso, cola_inicial);
        }
  }
  printf("Liberando memoria...\n");
  input_file_destroy(file);

  //printf("Primer proceso: %s\n ", cola_inicial->primer_proceso->nombre);
  //printf("Segundo proceso: %s \n", cola_seccion3->primer_proceso->nombre);


  // Instanciar Plani <3

  int time = 0;
  while (cola_final->largo == total_length){
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
    
    if (process_running){ // o if (running_process->estado == RUNNING){
      // --- Se descuenta 1 a A y a quantum
      running_process->A -= 1;
      running_process->quantum -= 1;

      if (running_process->A == 0 && running_process->number_burst != (running_process->actual_burst+2)/2) {
        // RUNNING -> WAITING
        running_process->estado = WAITING;
        running_process->actual_burst += 1;
        running_process->B = running_process->array_burst[running_process->actual_burst];
        cambiar_seccion(running_process, running_process->section, 4, cola_secciones);
        if (running_process->quantum == 0){
          //running_process.interrupciones += 1;
        }
        // Buscar cual entra --------------- FUNCION -----------------
        // READY -> RUNNING
        Process* new_running = buscar_proceso_running(cola_secciones);
        if (new_running != NULL){
          new_running->A -= 1;
          new_running->quantum -= 1;
        }

      } else if (running_process->A == 0 && running_process->number_burst == (running_process->actual_burst+2)/2){
        // RUNNING -> FINISHED
        running_process->estado = FINISHED;
        finalizar_proceso(running_process, running_process->section, cola_final, cola_secciones);
        // Buscar cual entra --------------- FUNCION -----------------
        // READY -> RUNNING
        Process* new_running = buscar_proceso_running(cola_secciones);
        if (new_running != NULL){
          new_running->A -= 1;
          new_running->quantum -= 1;
        }

      } else if (running_process->quantum == 0){
        // RUNNING -> READY
        running_process->estado = READY;
        cambiar_seccion(running_process,running_process->section, 2, cola_secciones);
        //Buscar cual entra --------------- FUNCION -----------------
        // READY -> RUNNING
        Process* new_running = buscar_proceso_running(cola_secciones);
        if (new_running != NULL){
          new_running->A -= 1;
          new_running->quantum -= 1;
        }
        
      } else {
        // RUNNING -> RUNNING
        // NOSE SI HAY QUE RESTARLOS ACA O ARRIBA

        // running_process->A -= 1;
        // running_process->quantum -= 1;
      }
    
    // Si no hay procesos en estado RUNNING
    } else {
      // READY -> RUNNING
      for (int i=0; i<3;i++){
        if (cola_secciones->seccion[i]->largo > 0 ){
          running_process = cola_secciones->seccion[i]->primer_proceso;
          break;
        }
      }
      running_process->estado = RUNNING;
      running_process->A -= 1;
      running_process->quantum -= 1;

    }

    // Descontar B de procesos waiting
    Process* proceso_waiting = cola_seccion4->primer_proceso;

    if (cola_seccion4->largo == 1){
      proceso_waiting->B -= 1;
      
      if (proceso_waiting->B == 0){
        // WAITING -> READY
        cambiar_seccion(proceso_waiting, 4, 1, cola_secciones);
        proceso_waiting->actual_burst += 1;
        proceso_waiting->A = proceso_waiting->array_burst[proceso_waiting->actual_burst];
      }
    
    } else {
      while (proceso_waiting->siguiente != NULL){
        proceso_waiting->B -= 1;

        if (proceso_waiting->B == 0){
          // WAITING -> READY
          cambiar_seccion(proceso_waiting, 4, 1, cola_secciones);
          proceso_waiting->actual_burst += 1;
          proceso_waiting->A = proceso_waiting->array_burst[proceso_waiting->actual_burst];
        }
        
        proceso_waiting = proceso_waiting->siguiente;
      }
      proceso_waiting->B -= 1; 
    }
    

    // CHEQUEO LA COLA INICAL         INICIAL -> READY
    if (cola_inicial->largo == 1){
      if (cola_inicial->primer_proceso->tiempo_llegada == time){
        insertar_proceso(cola_inicial->primer_proceso, cola_seccion3);
      }
    
    } else {
      int cuantos_entran = 0;  
      Process* proceso_sin_iniciar = cola_inicial->primer_proceso;
      while(proceso_sin_iniciar->siguiente != NULL){
        if (proceso_sin_iniciar->tiempo_llegada == time){
          cuantos_entran += 1;
          //Guardar su puntero (TAMI)
        }
      }
      if (cuantos_entran == 1){
        insertar_proceso(puntero, cola_seccion3);
      } else if (cuantos_entran > 1){
        prioridad(puntero_1, puntero_2);
      } else {}
    }

  time += 1;
  }
  
  // while (final_queue->length == total_length){
  //   // Checkear si hay un proceso en running (si hay)
  //     // 1. Se ejecuta la funcion CPU
  //     
      
  //   // Si no hay proceso RUNNING
  //     // 1. Buscar en lista estados, algun proceso "READY"
  //     // Si hay un proceso "READY", 
  //         //pasarlo a "RUNNING", ejecutar la funcion CPU
  //         // llevar estadistica


  //   // 2. Contadores (for de la lista estados) y estadistica
  //     // --- "RUNNING"
  //       // --- Checkear nuevos estados
  //       // --- Si A o quantum = 0, hago blabla
  //       // --- cachar si pasa a ready (seccion 2) o waiting o finished (no quede un B; igualdad rafagas)
      
  //     // --- "READY"
  //       // nada
      
  //     // --- "WAITING"
  //       // --- Se descuenta 1 a B y chequear nuevo estado 
  //       // --- Si B es 0 pasa a "READY" Section 1


  //   //
  //   time += 1;
  //   // Agregar desde fila_inical a fila_estado que cumplan ocn que tiempo_llegada = tiempo
  // }

  // Escribir el Output

  // Liberar la memoria
  
  destroy_queue(cola_inicial);
  destroy_queue(cola_seccion1);
  destroy_queue(cola_seccion2);
  destroy_queue(cola_seccion3);
  destroy_queue(cola_seccion4);
  destroy_queue(cola_final);
  destroy_queue_secciones(cola_secciones);
}
