#ifndef SINCRONIZACAO_H
#define SINCRONIZACAO_H

#include <pthread.h>

// Mutexes para recursos compartilhados
extern pthread_mutex_t mutex_helicoptero;   // Protege acesso ao helicóptero
extern pthread_mutex_t mutex_foguetes;      // Protege lista de foguetes
extern pthread_mutex_t mutex_bateria_0;     // Protege estado da bateria 0
extern pthread_mutex_t mutex_bateria_1;     // Protege estado da bateria 1
extern pthread_mutex_t mutex_soldados;      // Protege contador de soldados
extern pthread_mutex_t mutex_deposito;      // Protege acesso ao depósito
extern pthread_mutex_t mutex_ponte;         // Protege acesso à ponte

// Variáveis de condição para depósito e ponte
extern pthread_cond_t cond_deposito;        // Sinaliza depósito livre
extern pthread_cond_t cond_ponte;           // Sinaliza ponte livre

// Inicialização e destruição dos mecanismos de sincronização
void inicializar_sincronizacao();
void destruir_sincronizacao();

#endif
