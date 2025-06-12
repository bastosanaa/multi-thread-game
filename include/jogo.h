#ifndef JOGO_H
#define JOGO_H

// Tamanho do cenário
#define ALTURA_TELA 20
#define LARGURA_TELA 60

// Número de soldados a resgatar
#define TOTAL_SOLDADOS 10

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

// Protótipos
void inicializar_jogo();

#endif // JOGO_H
