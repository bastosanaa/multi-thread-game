#include "../include/sincronizacao.h"

// Definição dos mutexes globais
pthread_mutex_t mutex_helicoptero;
pthread_mutex_t mutex_foguetes;
pthread_mutex_t mutex_bateria_0;
pthread_mutex_t mutex_bateria_1;
pthread_mutex_t mutex_soldados;
pthread_mutex_t mutex_deposito;
pthread_mutex_t mutex_ponte;

// Definição das variáveis de condição
pthread_cond_t cond_deposito;
pthread_cond_t cond_ponte;

void inicializar_sincronizacao() {
    pthread_mutex_init(&mutex_helicoptero, NULL);
    pthread_mutex_init(&mutex_foguetes, NULL);
    pthread_mutex_init(&mutex_bateria_0, NULL);
    pthread_mutex_init(&mutex_bateria_1, NULL);
    pthread_mutex_init(&mutex_soldados, NULL);
    pthread_mutex_init(&mutex_deposito, NULL);
    pthread_mutex_init(&mutex_ponte, NULL);

    pthread_cond_init(&cond_deposito, NULL);
    pthread_cond_init(&cond_ponte, NULL);
}

void destruir_sincronizacao() {
    pthread_mutex_destroy(&mutex_helicoptero);
    pthread_mutex_destroy(&mutex_foguetes);
    pthread_mutex_destroy(&mutex_bateria_0);
    pthread_mutex_destroy(&mutex_bateria_1);
    pthread_mutex_destroy(&mutex_soldados);
    pthread_mutex_destroy(&mutex_deposito);
    pthread_mutex_destroy(&mutex_ponte);

    pthread_cond_destroy(&cond_deposito);
    pthread_cond_destroy(&cond_ponte);
}
