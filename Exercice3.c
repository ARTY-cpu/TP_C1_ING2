#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5   // taille du tampon
char tampon[N];

pthread_mutex_t mutex;
pthread_cond_t cond_not_full;
pthread_cond_t cond_not_empty;

int in = 0;
int out = 0;
int count = 0;


void *ecrire_tampon(void *arg) {

    char c = *((char*)arg);

    pthread_mutex_lock(&mutex);

    while (count == N) {
        pthread_cond_wait(&cond_not_full, &mutex);
    }

    tampon[in] = c;
    in = (in + 1) % N;
    count++;

    pthread_cond_signal(&cond_not_empty);

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *lecture_tampon(void *arg) {

    char c;


    pthread_mutex_lock(&mutex);
    while (count == 0) {
        pthread_cond_wait(&cond_not_empty, &mutex);
    }

    c = tampon[out];
    out = (out + 1) % N;
    count--;

    pthread_cond_signal(&cond_not_full);

    pthread_mutex_unlock(&mutex);

    printf("Lu: %c\n", c);  // affichage pour test
    return NULL;
}


int main() {
    pthread_t t1, t2;
    char data = 'A';

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_not_full, NULL);
    pthread_cond_init(&cond_not_empty, NULL);

    pthread_create(&t1, NULL, ecrire_tampon, &data);
    pthread_create(&t2, NULL, lecture_tampon, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_not_full);
    pthread_cond_destroy(&cond_not_empty);

    return 0;
}