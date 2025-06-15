#ifndef JOGO_H
#define JOGO_H

#include "dificuldade.h"

// Tamanho do cenário
#define ALTURA_TELA 20
#define LARGURA_TELA 60

// Número de soldados a resgatar DEBUG
#define TOTAL_SOLDADOS 2
// #define TOTAL_SOLDADOS 10

// capacidade maxima do helicóptero
#define MAX_SOLDADOS_HELICOPTERO 1



// Direções
#define DIR_CIMA     1
#define DIR_BAIXO    2
#define DIR_ESQUERDA 3
#define DIR_DIREITA  4

// Estados do jogo
typedef enum {
    EM_ANDAMENTO,
    VITORIA,
    DERROTA
} EstadoJogo;

// Estrutura do helicóptero
typedef struct {
    int x, y; // posição
    int soldados_a_bordo;
} Helicoptero;

// Estrutura de um soldado
typedef struct {
    int x, y;
    int resgatado;
} Soldado;

// Estrutura de uma bateria
typedef struct {
    int x, y;
    int foguetes_restantes;
    int em_recarga;
} Bateria;

// Estrutura de um foguete
typedef struct {
    int x, y;
    int ativo;
    int direcao; // 1 = direita, -1 = esquerda
} Foguete;

// Variáveis globais do jogo
extern Helicoptero helicoptero;
extern Soldado soldados[TOTAL_SOLDADOS];
extern int soldados_resgatados;
extern EstadoJogo estado_jogo;

// Adicione as flags de ocupação e o vetor de baterias
extern int ponte_ocupada;
extern int deposito_ocupado;
extern Bateria baterias[2];

// Protótipos
void inicializar_jogo();
void inicializar_jogo_com_dificuldade(NivelDificuldade nivel);

void verificar_vitoria_fase();

#endif 