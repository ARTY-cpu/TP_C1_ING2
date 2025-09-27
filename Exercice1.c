#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>

sem_t s1, s2;

void *thread1nonsynchro() {
    sem_wait(&s1);
    printf("je sais "); fflush(stdout);
    sem_post(&s2);

    sem_wait(&s1);
    printf("ce type "); fflush(stdout);
    sem_post(&s2);

    return NULL;
}

void *thread2nonsynchro() {
    sem_wait(&s2);
    printf("toujours faire "); fflush(stdout);
    sem_post(&s1);

    sem_wait(&s2);
    printf("de synchronisation !\n");

    return NULL;
}


int main(void) {
    sem_init(&s1, 0, 1);
    sem_init(&s2, 0, 0);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1nonsynchro, NULL);
    pthread_create(&t2, NULL, thread2nonsynchro, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&s1);
    sem_destroy(&s2);

    return 0;
}