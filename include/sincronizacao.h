#ifndef SINCRONIZACAO_H
#define SINCRONIZACAO_H

#include <pthread.h>

// Mutexes para recursos compartilhados
extern pthread_mutex_t mutex_helicoptero;
extern pthread_mutex_t mutex_foguetes;
extern pthread_mutex_t mutex_bateria_0;
extern pthread_mutex_t mutex_bateria_1;
extern pthread_mutex_t mutex_soldados;
extern pthread_mutex_t mutex_deposito;
extern pthread_mutex_t mutex_ponte;

// Variáveis de condição para depósito e ponte
extern pthread_cond_t cond_deposito;
extern pthread_cond_t cond_ponte;

// Funções de inicialização e destruição
void inicializar_sincronizacao();
void destruir_sincronizacao();

#endif
