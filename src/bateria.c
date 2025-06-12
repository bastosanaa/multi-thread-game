#include "../include/bateria.h"
#include "../include/jogo.h"
#include "../include/foguete.h"
#include "../include/sincronizacao.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// Parâmetros de dificuldade (exemplo)
#define FOGUETES_POR_BATERIA 5
#define TEMPO_RECARGA_MIN 100000 // 0.1s
#define TEMPO_RECARGA_MAX 500000 // 0.5s

extern Bateria baterias[2]; // Defina em jogo.c se necessário

void* thread_bateria(void* arg) {
    int id = (int)(size_t)arg;
    Bateria* bat = &baterias[id];
    int direcao_foguete = (id == 0) ? 1 : -1;

    srand(time(NULL) + id * 100);

    while (1) {
        // Disparo de foguetes
        pthread_mutex_t* mutex_bat = (id == 0) ? &mutex_bateria_0 : &mutex_bateria_1;
        pthread_mutex_lock(mutex_bat);
        if (bat->foguetes_restantes > 0 && !bat->em_recarga) {
            // Dispara foguete
            criar_foguete(id, bat->x, bat->y, direcao_foguete);
            bat->foguetes_restantes--;
            pthread_mutex_unlock(mutex_bat);
            usleep(300000 + rand() % 200000); // Delay aleatório entre disparos
        } else {
            pthread_mutex_unlock(mutex_bat);

            // Precisa recarregar
            // Travessia da ponte (exclusão mútua)
            pthread_mutex_lock(&mutex_ponte);
            while (/* ponte ocupada por outra bateria */ 0) {
                pthread_cond_wait(&cond_ponte, &mutex_ponte);
            }
            // Marca ponte como ocupada (implemente flag se necessário)
            pthread_mutex_unlock(&mutex_ponte);

            // Chega ao depósito (exclusão mútua)
            pthread_mutex_lock(&mutex_deposito);
            while (/* depósito ocupado */ 0) {
                pthread_cond_wait(&cond_deposito, &mutex_deposito);
            }
            // Marca depósito como ocupado (implemente flag se necessário)
            bat->em_recarga = 1;
            pthread_mutex_unlock(&mutex_deposito);

            // Tempo de recarga aleatório
            int tempo = TEMPO_RECARGA_MIN + rand() % (TEMPO_RECARGA_MAX - TEMPO_RECARGA_MIN + 1);
            usleep(tempo);

            // Finaliza recarga
            pthread_mutex_lock(mutex_bat);
            bat->foguetes_restantes = FOGUETES_POR_BATERIA;
            bat->em_recarga = 0;
            pthread_mutex_unlock(mutex_bat);

            // Libera depósito
            pthread_mutex_lock(&mutex_deposito);
            // Marca depósito como livre
            pthread_cond_signal(&cond_deposito);
            pthread_mutex_unlock(&mutex_deposito);

            // Libera ponte
            pthread_mutex_lock(&mutex_ponte);
            // Marca ponte como livre
            pthread_cond_signal(&cond_ponte);
            pthread_mutex_unlock(&mutex_ponte);

            // Volta para posição de disparo
            usleep(200000);
        }
    }
    return NULL;
}
