#include "../include/jogo.h"

// Variáveis globais
Helicoptero helicoptero;
Soldado soldados[TOTAL_SOLDADOS];
int soldados_resgatados = 0;
EstadoJogo estado_jogo = EM_ANDAMENTO;

void inicializar_jogo() {
    // Inicializa helicóptero na posição inicial (esquerda, meio da tela)
    helicoptero.x = 2;
    helicoptero.y = ALTURA_TELA / 2;
    helicoptero.soldados_a_bordo = 0;

    // Inicializa soldados na lateral esquerda, espaçados verticalmente
    for (int i = 0; i < TOTAL_SOLDADOS; i++) {
        soldados[i].x = 1;
        soldados[i].y = 2 + i * ((ALTURA_TELA-4) / TOTAL_SOLDADOS);
        soldados[i].resgatado = 0;
    }

    soldados_resgatados = 0;
    estado_jogo = EM_ANDAMENTO;
}
