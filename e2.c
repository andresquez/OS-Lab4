#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
// el tenedor de la izquierda es el tenedor con el id del filosofo + 1
#define LEFT_FORK (id + 1) % NUM_PHILOSOPHERS
// el tenedor de la derecha es el tenedor con el id del filosofo
#define RIGHT_FORK id

pthread_mutex_t forks[NUM_PHILOSOPHERS];
pthread_t philosophers[NUM_PHILOSOPHERS];
pthread_mutex_t print_lock;

void *philosopher(void *arg) {
    int id = *((int *)arg);
    int bites = 5; // Cantidad de veces que va a comer el filo

    while (bites > 0) {
        // Esperando
        usleep(rand() % 1000);

        // Intentar adquirir tenedores
        pthread_mutex_lock(&forks[LEFT_FORK]);
        pthread_mutex_lock(&forks[RIGHT_FORK]);

        // Comer
        pthread_mutex_lock(&print_lock);
        printf("Philosopher %d is eating.\n", id);
        pthread_mutex_unlock(&print_lock);

        usleep(rand() % 1000);

        pthread_mutex_lock(&print_lock);
        printf("Philosopher %d finished eating.\n", id);
        pthread_mutex_unlock(&print_lock);

        // Release los tenedores
        pthread_mutex_unlock(&forks[RIGHT_FORK]);
        pthread_mutex_unlock(&forks[LEFT_FORK]);

        bites--;
    }

    return NULL;
}

int main() {
    int ids[NUM_PHILOSOPHERS];

    // Inicializar mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }
    pthread_mutex_init(&print_lock, NULL);

    // Crear hilos para los filosofos 
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Liberar mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
    pthread_mutex_destroy(&print_lock);

    return 0;
}
