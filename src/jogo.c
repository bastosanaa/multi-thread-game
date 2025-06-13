#include "../include/jogo.h"

// Variáveis globais
Helicoptero helicoptero;
Soldado soldados[TOTAL_SOLDADOS];
int soldados_resgatados = 0;
EstadoJogo estado_jogo = EM_ANDAMENTO;

// Flags de ocupação e vetor de baterias
int ponte_ocupada = 0;
int deposito_ocupado = 0;
Bateria baterias[2];

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

    // Inicializa baterias (exemplo de posições)
    baterias[0].x = LARGURA_TELA / 4;
    baterias[0].y = ALTURA_TELA - 2;
    baterias[0].foguetes_restantes = 5;
    baterias[0].em_recarga = 0;

    baterias[1].x = (LARGURA_TELA / 4) * 3;
    baterias[1].y = ALTURA_TELA - 2;
    baterias[1].foguetes_restantes = 5;
    baterias[1].em_recarga = 0;

    ponte_ocupada = 0;
    deposito_ocupado = 0;

    soldados_resgatados = 0;
    estado_jogo = EM_ANDAMENTO;
}
