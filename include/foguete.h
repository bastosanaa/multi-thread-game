#ifndef FOGUETE_H
#define FOGUETE_H

// Função da thread dos foguetes
void* thread_foguetes(void* arg);

// Função para solicitar criação de um novo foguete
void criar_foguete(int bateria_id, int x, int y, int direcao);

#endif
