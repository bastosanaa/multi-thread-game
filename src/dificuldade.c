#include "../include/dificuldade.h"

NivelDificuldade nivel_atual = FACIL;
ConfigDificuldade config_atual;
int fase_atual = 1;

static ConfigDificuldade configs[4] = {
    {0, 0, 0, 0, 0, 0}, 
    // FÁCIL - Fase 1
    {10, 1500000, 2000000, 800000, 1200000, 3},  // velocidade 1 (lenta)
    // MÉDIO - Fase 2  
    {20, 800000, 1200000, 400000, 700000, 5},    // velocidade 2 (média)
    // DIFÍCIL - Fase 3
    {30, 300000, 600000, 200000, 400000, 8}     // velocidade 3 (rápida)
};

void definir_dificuldade(NivelDificuldade nivel) {
    if (nivel >= FACIL && nivel <= DIFICIL) {
        nivel_atual = nivel;
        carregar_configuracao_dificuldade();
    }
}

void carregar_configuracao_dificuldade() {
    config_atual = configs[nivel_atual];
}

void avancar_fase() {
    if (nivel_atual < DIFICIL) {
        nivel_atual++;
        fase_atual++;
        carregar_configuracao_dificuldade();
    }
}

int jogo_completo() {
    // CORRIGIDO: Jogo completo quando está no nível DIFÍCIL e completou essa fase
    return (nivel_atual == DIFICIL);
}