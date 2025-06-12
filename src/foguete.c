#include "../include/foguete.h"
#include "../include/jogo.h"
#include "../include/sincronizacao.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_FOGUETES 32

Foguete lista_foguetes[MAX_FOGUETES];

void criar_foguete(int bateria_id, int x, int y, int direcao) {
    pthread_mutex_lock(&mutex_foguetes);
    for (int i = 0; i < MAX_FOGUETES; i++) {
        if (!lista_foguetes[i].ativo) {
            lista_foguetes[i].x = x;
            lista_foguetes[i].y = y;
            lista_foguetes[i].ativo = 1;
            lista_foguetes[i].direcao = direcao;
            break;
        }
    }
    pthread_mutex_unlock(&mutex_foguetes);
}

void* thread_foguetes(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex_foguetes);
        for (int i = 0; i < MAX_FOGUETES; i++) {
            if (lista_foguetes[i].ativo) {
                // Move foguete
                lista_foguetes[i].x += lista_foguetes[i].direcao;
                // Remove se saiu da tela
                if (lista_foguetes[i].x < 0 || lista_foguetes[i].x >= LARGURA_TELA) {
                    lista_foguetes[i].ativo = 0;
                }
                // Checa colisão com helicóptero
                pthread_mutex_lock(&mutex_helicoptero);
                if (lista_foguetes[i].x == helicoptero.x && lista_foguetes[i].y == helicoptero.y) {
                    estado_jogo = DERROTA;
                    lista_foguetes[i].ativo = 0;
                }
                pthread_mutex_unlock(&mutex_helicoptero);
            }
        }
        pthread_mutex_unlock(&mutex_foguetes);
        usleep(50000);
    }
    return NULL;
}
