#include "../include/bateria.h"
#include "../include/jogo.h"
#include "../include/foguete.h"
#include "../include/sincronizacao.h"
#include "../include/dificuldade.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void* thread_bateria(void* arg) {
    int id = (int)(size_t)arg;
    Bateria* bat = &baterias[id];
    int direcao_foguete = -1; 
    static int direcao_movimento[2] = {1, -1};

    srand(time(NULL) + id * 100);

    while (estado_jogo == EM_ANDAMENTO) {
        pthread_mutex_t* mutex_bat = (id == 0) ? &mutex_bateria_0 : &mutex_bateria_1;
        
        // CORRIGIDO: velocidade dentro do loop
        int velocidade_movimento = config_atual.velocidade_movimento;
        
        pthread_mutex_lock(mutex_bat);
        
        if (bat->foguetes_restantes > 0 && !bat->em_recarga) {
            criar_foguete(id, bat->x, bat->y, direcao_foguete);
            bat->foguetes_restantes--;
            printf("Bateria %d disparou! Foguetes restantes: %d\n", id, bat->foguetes_restantes);
            
            // MOVIMENTO DA BATERIA
            bat->x += direcao_movimento[id] * velocidade_movimento;
            
            // Verifica limites e inverte direção se necessário
            if (bat->x <= 5) {  // Margem da esquerda
                direcao_movimento[id] = 1; // Vai para direita
            } else if (bat->x >= LARGURA_TELA - 5) {  // Margem da direita
                direcao_movimento[id] = -1; // Vai para esquerda
            }
            
            pthread_mutex_unlock(mutex_bat);
    
            // Usa configuração de dificuldade para intervalo de disparo
            int intervalo = config_atual.intervalo_disparo_min + 
                        rand() % (config_atual.intervalo_disparo_max - config_atual.intervalo_disparo_min + 1);
            usleep(intervalo);
    
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

            // Usa configuração de dificuldade para tempo de recarga
            int tempo = config_atual.tempo_recarga_min + 
                    rand() % (config_atual.tempo_recarga_max - config_atual.tempo_recarga_min + 1);
            usleep(tempo);

            // Finaliza recarga
            pthread_mutex_lock(mutex_bat);
            bat->foguetes_restantes = config_atual.foguetes_por_bateria;
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