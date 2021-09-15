#include <stdio.h>
#include "entidades.h"
#include "../file_manager/manager.h"


int main(int argc, char **argv)
{
  printf("Hello T2!\n");

  InputFile *file = read_file("input.txt");
  
  Queue* final_queue = queque_init();
  int total_length = file->len;

  printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
    printf(
        "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
        line[0], line[2], line[1], line[3]);
        // Poblar array de procesos iniciales
        // armar por id
        // Si tiempo de llegada = 0, mandar a otro array, cambiar el atributo section => 3
  }
  // Instanciar Plani <3
  // Armar fila estados, con las secciones respectivas (TAMI)

  int time = 0;
  while (final_queue->length == total_length){
    // Checkear si hay un proceso en running (si hay)
      // 1. Se ejecuta la funcion CPU
      // --- Se descuenta 1 a A y a quantum
      
    // Si no hay proceso RUNNING
      // 1. Buscar en lista estados, algun proceso "READY"
      // Si hay un proceso "READY", 
          //pasarlo a "RUNNING", ejecutar la funcion CPU


    // 2. Contadores (for de la lista estados) y estadistica
      // --- "RUNNING"
        // --- Checkear nuevos estados
        // --- Si A o quantum = 0, hago blabla
        // --- cachar si pasa a ready (seccion 2) o waiting o finished (no quede un B; igualdad rafagas)
      
      // --- "READY"
        // nada
      
      // --- "WAITING"
        // --- Se descuenta 1 a B y chequear nuevo estado 
        // --- Si B es 0 pasa a "READY" Section 1


    //
    time += 1;
    // Agregar desde fila_inical a fila_estado que cumplan ocn que tiempo_llegada = tiempo
  }



}
