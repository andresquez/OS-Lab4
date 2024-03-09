// Universidad del Valle de Guatemala
// Sistemas Operativos
// Lab4 - Ejercicio 1
// Andres Quezada 21085

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t resource1_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resource2_mutex = PTHREAD_MUTEX_INITIALIZER;

void *process1(void *arg) {
    while (1) {
        printf("Proceso 1 intentando adquirir recurso 1...\n");
        pthread_mutex_lock(&resource1_mutex);
        printf("Proceso 1 adquirió recurso 1.\n");

        // Simular procesamiento
        sleep(1);

        printf("Proceso 1 intentando adquirir recurso 2...\n");
        pthread_mutex_lock(&resource2_mutex);
        printf("Proceso 1 adquirió recurso 2.\n");

        // Simular procesamiento
        sleep(1);

        // Liberar recursos
        pthread_mutex_unlock(&resource2_mutex);
        pthread_mutex_unlock(&resource1_mutex);
    }
}

void *process2(void *arg) {
    while (1) {
        printf("Proceso 2 intentando adquirir recurso 2...\n");
        pthread_mutex_lock(&resource2_mutex);
        printf("Proceso 2 adquirió recurso 2.\n");

        // Simular procesamiento
        sleep(1);

        printf("Proceso 2 intentando adquirir recurso 1...\n");
        pthread_mutex_lock(&resource1_mutex);
        printf("Proceso 2 adquirió recurso 1.\n");

        // Simular procesamiento
        sleep(1);

        // Liberar recursos
        pthread_mutex_unlock(&resource1_mutex);
        pthread_mutex_unlock(&resource2_mutex);
    }
}

int main() {
    pthread_t tid1, tid2;
    
    // Crear los hilos para los procesos
    pthread_create(&tid1, NULL, process1, NULL);
    pthread_create(&tid2, NULL, process2, NULL);

    // Esperar a que los hilos terminen (esto nunca sucede porque los hilos están en bucles infinitos)
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}
