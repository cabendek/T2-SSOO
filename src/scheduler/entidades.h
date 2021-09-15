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
    int cant_rafagas;
    struct proceso* siguiente;
    int* burst;
} Process;

typedef struct queque{
    int length;
    struct proceso* primero;
    struct proceso* ultimo;
} Queue;


Process* process_init(int pid, char* nombre, int fabrica, int tiempo_llegada, int* burst);