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
void* thread_helicoptero(void* /*arg*/) {
    int running = 1;
    while (running) {
        // Captura entrada do usuário (não bloqueante)
        int tecla = getch();

        // Determina direção
        int direcao = 0;
        if (tecla == 'w') direcao = DIR_CIMA;
        else if (tecla == 's') direcao = DIR_BAIXO;
        else if (tecla == 'a') direcao = DIR_ESQUERDA;
        else if (tecla == 'd') direcao = DIR_DIREITA;
        else if (tecla == 27) { // Sequência de escape para setas
            int c1 = getch();
            if (c1 == 91) {
                int c2 = getch();
                switch (c2) {
                    case 'A': direcao = DIR_CIMA; break;    // Seta para cima
                    case 'B': direcao = DIR_BAIXO; break;   // Seta para baixo
                    case 'C': direcao = DIR_DIREITA; break; // Seta para direita
                    case 'D': direcao = DIR_ESQUERDA; break;// Seta para esquerda
                }
            }
        }
        else direcao = 0;

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
        usleep(80000);
    }
    return NULL;
}

// Função para mover o helicóptero
void mover_helicoptero(int direcao) {
    int nx = helicoptero.x, ny = helicoptero.y;
    
    // Calcula nova posição baseada na direção
    switch(direcao) {
        case DIR_CIMA:
            ny--; // permite ir para linha 0 (topo)
            break;
        case DIR_BAIXO:
            ny++; // permite ir para última linha (solo)
            break;
        case DIR_ESQUERDA:
            nx--; // permite ir para primeira coluna (parede)
            break;
        case DIR_DIREITA:
            nx++; // permite ir para última coluna (parede)
            break;
    }
    
    // Atualiza posição
    helicoptero.x = nx;
    helicoptero.y = ny;
}

// Função para checar colisão do helicóptero
int checar_colisao_helicoptero() {
    // Colisão com topo da tela - EXPLODE
    if (helicoptero.y <= 0) {
        return 1;
    }
    
    // Colisão com solo - EXPLODE  
    if (helicoptero.y >= ALTURA_TELA-1) {
        return 1;
    }
    
    // Colisão com bordas laterais - EXPLODE
    if (helicoptero.x <= 0 || helicoptero.x >= LARGURA_TELA-1) {
        return 1;
    }

    // Colisão com baterias - EXPLODE (protegido por mutex)
    pthread_mutex_lock(&mutex_bateria_0);
    int colidiu_bat0 = (helicoptero.x == baterias[0].x && helicoptero.y == baterias[0].y);
    pthread_mutex_unlock(&mutex_bateria_0);
    
    pthread_mutex_lock(&mutex_bateria_1);
    int colidiu_bat1 = (helicoptero.x == baterias[1].x && helicoptero.y == baterias[1].y);
    pthread_mutex_unlock(&mutex_bateria_1);
    
    if (colidiu_bat0 || colidiu_bat1) {
        return 1;
    }
    
    // Colisão com depósito - EXPLODE
    if (helicoptero.x == 3 && helicoptero.y == ALTURA_TELA/4) {
        return 1;
    }
    
    // Colisão com plataforma de resgate - NÃO EXPLODE (é o objetivo)
    if (helicoptero.x == LARGURA_TELA-2 && helicoptero.y == ALTURA_TELA/2) {
        return 0; // Não explode, é o local de resgate
    }

    return 0;
}

// Função para processar resgate de soldados
void processar_resgate() {
    static int ultima_posicao_x = -1;
    static int ultima_posicao_y = -1;
    static int ja_pegou_aqui = 0;

    // detecta se mudou de posição    
    if (helicoptero.x != ultima_posicao_x || helicoptero.y != ultima_posicao_y) {
        ultima_posicao_x = helicoptero.x;
        ultima_posicao_y = helicoptero.y;
        ja_pegou_aqui = 0; // Resetar flag se mudou de posição
    }

    // Se pode pegar soldado: não pegou nesta posição e tem espaço no helicóptero
    if (helicoptero.x == LARGURA_TELA-2 && helicoptero.y == ALTURA_TELA/2 && helicoptero.soldados_a_bordo > 0) {
        pthread_mutex_lock(&mutex_soldados);
        soldados_resgatados += helicoptero.soldados_a_bordo;
        helicoptero.soldados_a_bordo = 0;
        printf("RESGATOU soldados na plataforma!\n");
        verificar_vitoria_fase();
        pthread_mutex_unlock(&mutex_soldados);
    }
    
    // Se está sobre um soldado, pega ele (um por vez)
    if (!ja_pegou_aqui && helicoptero.soldados_a_bordo < MAX_SOLDADOS_HELICOPTERO) {
        for (int i = 0; i < TOTAL_SOLDADOS; i++) {
            if (!soldados[i].resgatado &&
                helicoptero.x == soldados[i].x && helicoptero.y == soldados[i].y) {
                pthread_mutex_lock(&mutex_soldados);
                soldados[i].resgatado = 1;
                helicoptero.soldados_a_bordo++;
                ja_pegou_aqui = 1; // Já pegou um soldado aqui
                printf("*** PEGOU soldado %d na posição (%d,%d)! Total a bordo: %d ***\n", i, helicoptero.x, helicoptero.y, helicoptero.soldados_a_bordo);
                if (helicoptero.soldados_a_bordo >= MAX_SOLDADOS_HELICOPTERO) {
                    printf("Helicóptero cheio! Volte para a plataforma para resgatar os soldados.\n");
                }
                pthread_mutex_unlock(&mutex_soldados);
                break;
            }
        }
    } 
    //se helicoptero esta cheio e há soldados na posição
    else if (helicoptero.soldados_a_bordo >= MAX_SOLDADOS_HELICOPTERO) {
        for(int i = 0; i < TOTAL_SOLDADOS; i++) {
            if (!soldados[i].resgatado &&
                helicoptero.x == soldados[i].x && helicoptero.y == soldados[i].y) {
                printf("*** HELICÓPTERO LOTADO (%d/%d)! Entregue na plataforma primeiro! ***\n", helicoptero.soldados_a_bordo, MAX_SOLDADOS_HELICOPTERO);
                break;
            }
        }
    }
    //se já pegou soldado nesta posição
    else if (ja_pegou_aqui) {
        for(int i = 0; i < TOTAL_SOLDADOS; i++) {
            if (!soldados[i].resgatado && helicoptero.x == soldados[i].x && helicoptero.y == soldados[i].y) {
                printf("*** JÁ PEGOU soldado nesta posição! Mova-se primeiro! ***\n");
                break;
            }
        }
    }
}