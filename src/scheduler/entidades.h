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
    int cantidad_elecciones_CPU;
    int cantidad_interrupciones;
    int tiempo_finalizacion;
    int tiempo_primera_atencion;
    int waiting;
} Process;

typedef struct queue{
    int largo;
    Process* primer_proceso;
    Process* ultimo_proceso;
} Queue;

typedef struct queue_secciones{
    Queue** seccion;
} Queue_secciones;

Process* process_init(char* nombre, int fabrica, int tiempo_llegada, int number_burst);
Process* buscar_proceso_running(Queue_secciones* cola_secciones);
int prioridad(Process* process_1, Process* process_2);
int quantum(int Q, int fabrica, Queue_secciones* cola_secciones);
Queue* queue_init();
Queue_secciones* queue_secciones_init(Queue* cola_seccion1,Queue* cola_seccion2,Queue* cola_seccion3,Queue* cola_seccion4);

void insertar_proceso(Process* proceso, Queue* cola);
void quitar_proceso(Process* proceso, Queue* cola);
void inicializar_proceso(Process* proceso, Queue* cola_inicial, Queue_secciones* cola_secciones,int time);
void cambiar_seccion(Process* proceso, int seccion_inicial, int seccion_final, Queue_secciones* cola_secciones, int time);
void finalizar_proceso(Process* proceso, int seccion_inicial, Queue* cola_final, Queue_secciones* cola_secciones, int time);
void destroy_proceso(Process* proceso);
void destroy_queue(Queue* cola);
void destroy_queue_secciones(Queue_secciones* cola);