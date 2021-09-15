#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum estado{
    RUNNING,
    READY,
    WAITING,
    FINISHED
} Estado;

typedef struct proceso{
    int pid;
    char* nombre;
    Estado estado;
    int id_fabrica;
    int tiempo_llegada;
    int* array_burst;
    int number_burst;
    int section;
    int quantum;
    int A;
    int B;
    int actual_burst;
    struct proceso* siguiente;
} Process;

typedef struct queque{
    int length;
    struct proceso* primero;
    struct proceso* ultimo;
} Queue;


Process* process_init(int pid, char* nombre, int fabrica, int tiempo_llegada, int* burst, int number_burst);
Queue* queue_init();