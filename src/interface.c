#include "../include/interface.h"
#include "../include/jogo.h"
#include "../include/sincronizacao.h"
#include "../include/dificuldade.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// Limpa a tela (ANSI escape)
static void limpar_tela() {
    printf("\033[2J\033[H");
}

// Desenha o cenário do jogo em ASCII
static void desenhar_tela() {
    limpar_tela();
    // Topo
    for (int y = 0; y < ALTURA_TELA; y++) {
        for (int x = 0; x < LARGURA_TELA; x++) {
            char c = ' ';
            // Bordas
            if (y == 0 || y == ALTURA_TELA-1) c = '-';
            else if (x == 0 || x == LARGURA_TELA-1) c = '|';
            // Helicóptero
            pthread_mutex_lock(&mutex_helicoptero);
            if (x == helicoptero.x && y == helicoptero.y) c = 'H';
            pthread_mutex_unlock(&mutex_helicoptero);
            // Soldados
            pthread_mutex_lock(&mutex_soldados);
            for (int i = 0; i < TOTAL_SOLDADOS; i++) {
                if (!soldados[i].resgatado && x == soldados[i].x && y == soldados[i].y)
                    c = 'S';
            }
            pthread_mutex_unlock(&mutex_soldados);
            // Baterias
            pthread_mutex_lock(&mutex_bateria_0);
            if (x == baterias[0].x && y == baterias[0].y) c = 'B';
            pthread_mutex_unlock(&mutex_bateria_0);
            pthread_mutex_lock(&mutex_bateria_1);
            if (x == baterias[1].x && y == baterias[1].y) c = 'B';
            pthread_mutex_unlock(&mutex_bateria_1);
             // Plataforma (direita)
            if (x == LARGURA_TELA-2 && y == ALTURA_TELA/2) c = 'P';
            
            // DEPÓSITO/ARMAZÉM (esquerda) - Representação maior
            if ((x >= 1 && x <= 4) && (y >= ALTURA_TELA-4 && y <= ALTURA_TELA-2)) {
                if ((x == 1 || x == 4) && (y == ALTURA_TELA/4 - 1 || y == ALTURA_TELA/4 + 1)) 
                    c = '+'; // Cantos
                else if (x == 1 || x == 4) 
                    c = '|'; // Paredes laterais
                else if (y == ALTURA_TELA-4 || y == ALTURA_TELA-2) 
                    c = '-'; // Teto e chão
                else if (x == 2 && y == ALTURA_TELA-3) 
                    c = 'D'; // Símbolo do depósito no centro
                else 
                    c = ' '; // Interior
            }
            
             // PONTE (horizontal) - Representação mais realista e mais larga
            if (y == ALTURA_TELA-3 && (x >= 8 && x <= 15)) {
                c = '='; // Tabuleiro da ponte
            }
            // Pilares da ponte
            if ((x == 8 || x == 15) && (y >= ALTURA_TELA-4 && y <= ALTURA_TELA-2)) {
                c = 'I'; // Pilares verticais
            }
            
            // Foguetes
            pthread_mutex_lock(&mutex_foguetes);
            extern Foguete lista_foguetes[]; // Definido em foguete.c
            for (int i = 0; i < 32; i++) {
                if (lista_foguetes[i].ativo && x == lista_foguetes[i].x && y == lista_foguetes[i].y)
                    c = '*';
            }
            pthread_mutex_unlock(&mutex_foguetes);
            printf("%c", c);
        }
        printf("\n");
    }
    // Status
    printf("Soldados resgatados: %d/%d\n", soldados_resgatados, TOTAL_SOLDADOS);
    printf("Foguetes B0: %d | B1: %d\n", baterias[0].foguetes_restantes, baterias[1].foguetes_restantes);
    printf("Fase %d - Dificuldade: %s\n", fase_atual,
        nivel_atual == FACIL ? "Fácil" : 
        nivel_atual == MEDIO ? "Médio" : "Difícil");
    printf("Status: ");
    if (estado_jogo == EM_ANDAMENTO) printf("Em andamento\n");
    else if (estado_jogo == VITORIA) printf("VITÓRIA TOTAL - Todas as fases completadas!\n");
    else printf("DERROTA!\n");
}

// Thread de interface
void* thread_interface(void* arg) {
    while (estado_jogo == EM_ANDAMENTO) {
        desenhar_tela();
        usleep(100000); // Atualiza a cada 0.1s
    }
    desenhar_tela(); // Mostra tela final
    return NULL;
}
