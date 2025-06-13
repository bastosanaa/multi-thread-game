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
    int direcao_foguete =  -1;

    int direcao_movimento = (id == 0) ? 1 : -1; // Bateria 0 vai para direita, 1 para esquerda
    int velocidade_movimento = 1; // Velocidade de movimento da bateria
    
    srand(time(NULL) + id * 100);

    while (1) {
        pthread_mutex_t* mutex_bat = (id == 0) ? &mutex_bateria_0 : &mutex_bateria_1;
        pthread_mutex_lock(mutex_bat);

        bat -> x += direcao_movimento * velocidade_movimento;

        if (bat -> x <= 1){
            direcao_movimento = 1; // Muda direção para direita
        } else if (bat -> x >= LARGURA_TELA - 2) {
            direcao_movimento = -1; // Muda direção para esquerda
        }

        if (bat->foguetes_restantes > 0 && !bat->em_recarga) {
            criar_foguete(id, bat->x, bat->y, direcao_foguete);
            bat->foguetes_restantes--;
            pthread_mutex_unlock(mutex_bat);
            usleep(300000 + rand() % 200000);
        } else {
            pthread_mutex_unlock(mutex_bat);

            // Travessia da ponte (exclusão mútua)
            pthread_mutex_lock(&mutex_ponte);
            while (ponte_ocupada) {
                pthread_cond_wait(&cond_ponte, &mutex_ponte);
            }
            ponte_ocupada = 1;
            pthread_mutex_unlock(&mutex_ponte);

            // Chega ao depósito (exclusão mútua)
            pthread_mutex_lock(&mutex_deposito);
            while (deposito_ocupado) {
                pthread_cond_wait(&cond_deposito, &mutex_deposito);
            }
            deposito_ocupado = 1;
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
            deposito_ocupado = 0;
            pthread_cond_signal(&cond_deposito);
            pthread_mutex_unlock(&mutex_deposito);

            // Libera ponte
            pthread_mutex_lock(&mutex_ponte);
            ponte_ocupada = 0;
            pthread_cond_signal(&cond_ponte);
            pthread_mutex_unlock(&mutex_ponte);

            usleep(200000);
        }
    }
    return NULL;
}
