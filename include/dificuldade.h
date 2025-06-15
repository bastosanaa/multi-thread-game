#ifndef DIFICULDADE_H
#define DIFICULDADE_H

typedef enum {
    FACIL = 1,
    MEDIO = 2,
    DIFICIL = 3
} NivelDificuldade;

typedef struct {
    int foguetes_por_bateria;
    int tempo_recarga_min;  
    int tempo_recarga_max;  
    int intervalo_disparo_min;  
    int intervalo_disparo_max;
    int velocidade_movimento;
} ConfigDificuldade;

extern NivelDificuldade nivel_atual;
extern ConfigDificuldade config_atual;
extern int fase_atual;

void definir_dificuldade(NivelDificuldade nivel);
void carregar_configuracao_dificuldade();
void avancar_fase(); // Nova função para progressão automática
int jogo_completo(); // Verifica se completou todas as fases

#endif