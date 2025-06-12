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
    // ...implementar lógica de movimentação e limites...
    // Exemplo:
    // if (direcao == DIR_CIMA && helicoptero.y > 0) helicoptero.y--;
    // if (direcao == DIR_BAIXO && helicoptero.y < ALTURA_TELA-1) helicoptero.y++;
    // if (direcao == DIR_ESQUERDA && helicoptero.x > 0) helicoptero.x--;
    // if (direcao == DIR_DIREITA && helicoptero.x < LARGURA_TELA-1) helicoptero.x++;
}

// Função para checar colisão do helicóptero
int checar_colisao_helicoptero() {
    // ...verificar colisão com foguetes, solo, obstáculos, topo...
    // Retorne 1 se houver colisão, 0 caso contrário
    return 0;
}

// Função para processar resgate de soldados
void processar_resgate() {
    // ...verificar se helicóptero está na plataforma e soldados a bordo...
    // ...incrementar soldados_resgatados se aplicável...
}