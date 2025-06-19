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
    int escolha = 0;
    printf("Escolha o nível de dificuldade:\n");
    printf("1 - Fácil\n2 - Médio\n3 - Difícil\n");
    while (escolha < 1 || escolha > 3) {
        printf("Digite o número da dificuldade desejada: ");
        scanf("%d", &escolha);
        if (escolha < 1 || escolha > 3) printf("Opção inválida. Tente novamente.\n");
    }
    NivelDificuldade nivel = (NivelDificuldade)escolha;
    printf("Iniciando jogo no nível: %s\n", nivel == FACIL ? "Fácil" : nivel == MEDIO ? "Médio" : "Difícil");

    // Inicializa mecanismos de sincronização (mutexes, cond vars)
    inicializar_sincronizacao();

    //sempre inicia no nível FÁCIL
    inicializar_jogo_com_dificuldade(nivel);

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