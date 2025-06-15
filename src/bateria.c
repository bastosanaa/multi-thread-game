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
            if (bat->x <= 17) {  // Margem da esquerda
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

            printf("Bateria %d indo para recarga...\n", id);

            // === FASE 1: MOVIMENTO PARA A PONTE ===
            pthread_mutex_lock(mutex_bat);
            // Move para a esquerda até chegar na ponte (posição x = 10, por exemplo)
            while (bat->x > 17) {
                bat->x -= 2; // Move para esquerda
                pthread_mutex_unlock(mutex_bat);
                usleep(50000); // Delay visual do movimento
                pthread_mutex_lock(mutex_bat);
            }
            pthread_mutex_unlock(mutex_bat);

            // === FASE 2: TRAVESSIA DA PONTE (EXCLUSÃO MÚTUA) ===
            pthread_mutex_lock(&mutex_ponte);
            while (ponte_ocupada) {
                printf("Bateria %d esperando ponte ficar livre...\n", id);
                pthread_cond_wait(&cond_ponte, &mutex_ponte);
            }
            ponte_ocupada = 1;
            printf("Bateria %d atravessando a ponte...\n", id);
            pthread_mutex_unlock(&mutex_ponte);

            // Simula travessia da ponte (movimento visual)
            pthread_mutex_lock(mutex_bat);
            while (bat->x > 0) {
                bat->x -= 1;
                pthread_mutex_unlock(mutex_bat);
                usleep(100000); // Movimento mais lento na ponte
                pthread_mutex_lock(mutex_bat);
            }
            pthread_mutex_unlock(mutex_bat);

            // === FASE 3: CHEGADA AO DEPÓSITO (EXCLUSÃO MÚTUA) ===
            pthread_mutex_lock(&mutex_deposito);
            while (deposito_ocupado) {
                printf("Bateria %d esperando depósito ficar livre...\n", id);
                pthread_cond_wait(&cond_deposito, &mutex_deposito);
            }
            deposito_ocupado = 1;
            
            pthread_mutex_lock(mutex_bat);
            bat->em_recarga = 1;
            printf("Bateria %d recarregando no depósito...\n", id);
            pthread_mutex_unlock(mutex_bat);
            pthread_mutex_unlock(&mutex_deposito);

            // === FASE 4: PROCESSO DE RECARGA ===
            int tempo = config_atual.tempo_recarga_min + 
                    rand() % (config_atual.tempo_recarga_max - config_atual.tempo_recarga_min + 1);
            usleep(tempo);

            // Finaliza recarga
            pthread_mutex_lock(mutex_bat);
            bat->foguetes_restantes = config_atual.foguetes_por_bateria;
            bat->em_recarga = 0;
            printf("Bateria %d recarga completa! Voltando...\n", id);
            pthread_mutex_unlock(mutex_bat);

            // === FASE 5: LIBERA DEPÓSITO ===
            pthread_mutex_lock(&mutex_deposito);
            deposito_ocupado = 0;
            pthread_cond_signal(&cond_deposito);
            pthread_mutex_unlock(&mutex_deposito);

            // === FASE 6: VOLTA PELA PONTE (MANTÉM EXCLUSÃO MÚTUA) ===
            // A ponte continua ocupada desde a ida, agora volta pela mesma
            pthread_mutex_lock(mutex_bat);
            while (bat->x < 16) { // Vai até após a ponte (x=15 é o final da ponte)
                bat->x += 1;
                pthread_mutex_unlock(mutex_bat);
                usleep(100000); // Movimento na ponte
                pthread_mutex_lock(mutex_bat);
            }
            pthread_mutex_unlock(mutex_bat);

            // === FASE 7: LIBERA PONTE (APENAS DEPOIS DE ATRAVESSAR COMPLETAMENTE) ===
            pthread_mutex_lock(&mutex_ponte);
            ponte_ocupada = 0;
            printf("Bateria %d liberou a ponte e voltou ao combate!\n", id);
            pthread_cond_signal(&cond_ponte);
            pthread_mutex_unlock(&mutex_ponte);

            usleep(200000);
        }
    }
    return NULL;
}