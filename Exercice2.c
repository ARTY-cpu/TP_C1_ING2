#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int arrived = 0;
pthread_mutex_t m1;
pthread_cond_t cond;
int N = 5;

void attente_aleatoire() {
    const int rd = rand() % 3;
    sleep(rd);
}


void *thread_group() {
    attente_aleatoire();

    pthread_mutex_lock(&m1);
    arrived++;
    if (arrived == N) {
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&m1);

    attente_aleatoire();

    return NULL;
}

void *thread_barriere() {
    attente_aleatoire();

    pthread_mutex_lock(&m1);
    while (arrived < N) {
        pthread_cond_wait(&cond, &m1);
    }
    pthread_mutex_unlock(&m1);

    printf(">>> Tous les threads ont atteint la barriere !\n");

    attente_aleatoire();

    return NULL;
}

int main() {
    pthread_t groupes[N], t_barriere;
    srand(time(NULL));

    pthread_mutex_init(&m1, NULL);
    pthread_cond_init(&cond, NULL);

    for (int i = 0; i < N; i++) {
        pthread_create(&groupes[i], NULL, thread_group, NULL);
    }
    pthread_create(&t_barriere, NULL, thread_barriere, NULL);


    pthread_join(t_barriere, NULL);
    for (int i = 0; i < N; i++) {
        pthread_join(groupes[i], NULL);
    }

    pthread_mutex_destroy(&m1);
    pthread_cond_destroy(&cond);

    return 0;
}