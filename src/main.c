#include "../include/jogo.h"
#include "../include/helicoptero.h"
#include "../include/bateria.h"
#include "../include/foguete.h"
#include "../include/interface.h"
#include "../include/sincronizacao.h"
#include "../include/dificuldade.h"
#include <pthread.h>
#include <stdio.h>

int main() {
    // REMOVE a seleção manual - sempre começa no FÁCIL
    printf("Iniciando jogo! Progressão automática: Fácil → Médio → Difícil\n");
    
    // Inicializa mecanismos de sincronização (mutexes, cond vars)
    inicializar_sincronizacao();

    //sempre inicia no nível FÁCIL
    inicializar_jogo_com_dificuldade(FACIL);

    pthread_t th_helicop, th_bateria0, th_bateria1, th_foguetes, th_interface;

    // Cria threads principais
    pthread_create(&th_helicop, NULL, thread_helicoptero, NULL);
    pthread_create(&th_bateria0, NULL, thread_bateria, (void*)0);
    pthread_create(&th_bateria1, NULL, thread_bateria, (void*)1);
    pthread_create(&th_foguetes, NULL, thread_foguetes, NULL);
    pthread_create(&th_interface, NULL, thread_interface, NULL);

    // Aguarda o término das threads
    pthread_join(th_helicop, NULL);
    pthread_join(th_bateria0, NULL);
    pthread_join(th_bateria1, NULL);
    pthread_join(th_foguetes, NULL);
    pthread_join(th_interface, NULL);

    // Libera recursos de sincronização
    destruir_sincronizacao();

    return 0;
}