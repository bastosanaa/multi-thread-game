#include "../include/helicoptero.h"
#include "../include/jogo.h"
#include "../include/sincronizacao.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

// Função auxiliar para capturar tecla sem buffer (modo raw)
static int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(0, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(0, TCSANOW, &oldt);
    return ch;
}

// Função da thread do helicóptero
void* thread_helicoptero(void* arg) {
    int running = 1;
    while (running) {
        // Captura entrada do usuário (não bloqueante)
        int tecla = getch();

        // Determina direção
        int direcao = 0;
        switch (tecla) {
            case 'w': direcao = DIR_CIMA; break;
            case 's': direcao = DIR_BAIXO; break;
            case 'a': direcao = DIR_ESQUERDA; break;
            case 'd': direcao = DIR_DIREITA; break;
            default: direcao = 0; break;
        }

        // Move helicóptero (protegido por mutex)
        if (direcao) {
            pthread_mutex_lock(&mutex_helicoptero);
            mover_helicoptero(direcao);
            pthread_mutex_unlock(&mutex_helicoptero);
        }

        // Verifica colisão (foguete, obstáculo, solo, topo)
        pthread_mutex_lock(&mutex_helicoptero);
        if (checar_colisao_helicoptero()) {
            estado_jogo = DERROTA;
            running = 0;
        }
        pthread_mutex_unlock(&mutex_helicoptero);

        // Processa resgate se aplicável
        pthread_mutex_lock(&mutex_helicoptero);
        processar_resgate();
        if (soldados_resgatados >= TOTAL_SOLDADOS) {
            estado_jogo = VITORIA;
            running = 0;
        }
        pthread_mutex_unlock(&mutex_helicoptero);

        // Pequeno delay para evitar uso excessivo de CPU
        usleep(50000);
    }
    return NULL;
}

// Função para mover o helicóptero
void mover_helicoptero(int direcao) {
    // Limites do cenário
    int nx = helicoptero.x, ny = helicoptero.y;
    if (direcao == DIR_CIMA && helicoptero.y > 1) ny--;
    if (direcao == DIR_BAIXO && helicoptero.y < ALTURA_TELA-2) ny++;
    if (direcao == DIR_ESQUERDA && helicoptero.x > 1) nx--;
    if (direcao == DIR_DIREITA && helicoptero.x < LARGURA_TELA-2) nx++;
    // Não permite sair da tela
    helicoptero.x = nx;
    helicoptero.y = ny;
}

// Função para checar colisão do helicóptero
int checar_colisao_helicoptero() {
    // Colisão com topo
    if (helicoptero.y == 0)
        return 1;
    // Colisão com solo
    if (helicoptero.y == ALTURA_TELA-1)
        return 1;
    // Colisão com baterias
    if ((helicoptero.x == baterias[0].x && helicoptero.y == baterias[0].y) ||
        (helicoptero.x == baterias[1].x && helicoptero.y == baterias[1].y))
        return 1;
    // Colisão com depósito
    if (helicoptero.x == 3 && helicoptero.y == ALTURA_TELA/4)
        return 1;
    // Colisão com plataforma
    if (helicoptero.x == LARGURA_TELA-2 && helicoptero.y == ALTURA_TELA/2)
        return 0; // Não explode, é objetivo
    // ...pode adicionar obstáculos extras aqui...
    return 0;
}

// Função para processar resgate de soldados
void processar_resgate() {
    // Se está na plataforma e tem soldados a bordo, resgata
    if (helicoptero.x == LARGURA_TELA-2 && helicoptero.y == ALTURA_TELA/2 && helicoptero.soldados_a_bordo > 0) {
        pthread_mutex_lock(&mutex_soldados);
        soldados_resgatados += helicoptero.soldados_a_bordo;
        helicoptero.soldados_a_bordo = 0;
        pthread_mutex_unlock(&mutex_soldados);
    }
    // Se está sobre um soldado, pega ele (um por vez)
    for (int i = 0; i < TOTAL_SOLDADOS; i++) {
        if (!soldados[i].resgatado &&
            helicoptero.x == soldados[i].x && helicoptero.y == soldados[i].y) {
            pthread_mutex_lock(&mutex_soldados);
            soldados[i].resgatado = 1;
            helicoptero.soldados_a_bordo++;
            pthread_mutex_unlock(&mutex_soldados);
            break;
        }
    }
}