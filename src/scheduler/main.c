#include <stdio.h>
#include "entidades.h"
#include "../file_manager/manager.h"

int main(int argc, char **argv)
{
  printf("Hello T2!\n");

  InputFile *file = read_file("input.txt");

  printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
    printf(
        "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
        line[0], line[2], line[1], line[3]);
  }
  Process* process = process_init(0,"nombre",1,2,[A,B,A,B,A]);

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
}
