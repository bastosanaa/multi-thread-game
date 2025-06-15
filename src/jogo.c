#include "../include/jogo.h"
#include "../include/dificuldade.h"
#include <stdlib.h>
#include <stdio.h>


// Variáveis globais
Helicoptero helicoptero;
Soldado soldados[TOTAL_SOLDADOS];
int soldados_resgatados = 0;
EstadoJogo estado_jogo = EM_ANDAMENTO;

// Flags de ocupação e vetor de baterias
Bateria baterias[2];

void inicializar_jogo_com_dificuldade(NivelDificuldade nivel) {
    // Define a dificuldade
    definir_dificuldade(nivel);
    
    // Chama inicialização normal
    inicializar_jogo();
    
    // Ajusta foguetes iniciais das baterias baseado na dificuldade
    baterias[0].foguetes_restantes = config_atual.foguetes_por_bateria;
    baterias[1].foguetes_restantes = config_atual.foguetes_por_bateria;
}


void inicializar_jogo() {
    // Inicializa helicóptero na posição inicial (esquerda, meio da tela)
    helicoptero.x = 2;
    helicoptero.y = ALTURA_TELA / 2;
    helicoptero.soldados_a_bordo = 0;

    // Inicializa soldados na lateral esquerda, espaçados verticalmente
    for (int i = 0; i < TOTAL_SOLDADOS; i++) {
        soldados[i].x = 1;
        soldados[i].y = 2 + i * ((ALTURA_TELA-4) / TOTAL_SOLDADOS);
        soldados[i].resgatado = 0;
    }

    // Inicializa baterias (exemplo de posições)
    baterias[0].x = LARGURA_TELA / 4;
    baterias[0].y = ALTURA_TELA - 2;
    baterias[0].foguetes_restantes = 5;
    baterias[0].em_recarga = 0;

    baterias[1].x = (LARGURA_TELA / 4) * 3;
    baterias[1].y = ALTURA_TELA - 2;
    baterias[1].foguetes_restantes = 5;
    baterias[1].em_recarga = 0;

    soldados_resgatados = 0;
    estado_jogo = EM_ANDAMENTO;
}

void verificar_vitoria_fase() {
    if (soldados_resgatados >= TOTAL_SOLDADOS) {
        if (jogo_completo()) {
            // Completou todas as fases - vitória final!
            estado_jogo = VITORIA;
        } else {
            // Avança para próxima fase
            avancar_fase();
            
            printf("*** FASE COMPLETADA! Avançando para Fase %d - %s ***\n", 
                   fase_atual, 
                   nivel_atual == FACIL ? "Fácil" : 
                   nivel_atual == MEDIO ? "Médio" : "Difícil");
            
            // Reinicia o jogo com nova dificuldade
            soldados_resgatados = 0;
            
            // Reposiciona soldados
            for (int i = 0; i < TOTAL_SOLDADOS; i++) {
                soldados[i].resgatado = 0;
                soldados[i].x = 1;
                soldados[i].y = 2 + i * ((ALTURA_TELA-4) / TOTAL_SOLDADOS);
            }
            
            // Reinicia AMBAS as baterias com nova configuração
            baterias[0].foguetes_restantes = config_atual.foguetes_por_bateria;
            baterias[0].em_recarga = 0;
            baterias[1].foguetes_restantes = config_atual.foguetes_por_bateria;
            baterias[1].em_recarga = 0;
            
            // Reinicia helicóptero
            helicoptero.x = 2;
            helicoptero.y = ALTURA_TELA / 2;
            helicoptero.soldados_a_bordo = 0;
        }
    }
}
