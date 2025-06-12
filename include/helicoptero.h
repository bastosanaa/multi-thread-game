#ifndef HELICOPTERO_H
#define HELICOPTERO_H

#include "jogo.h"

// Função da thread do helicóptero
void* thread_helicoptero(void* arg);

// Funções auxiliares (implementação sugerida em helicoptero.c)
void mover_helicoptero(int direcao);
int checar_colisao_helicoptero();
void processar_resgate();

#endif // HELICOPTERO_H
