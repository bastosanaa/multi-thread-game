# Trabalho de Sistemas Operacionais – Contextualização para CODEX  
> **Arquivo-alvo:** `AGENTS.md`  
> **Propósito:** Fornecer a Codex/GPT a visão geral, requisitos e diretrizes de implementação do jogo “Resgate de Helicóptero” para que as sugestões de código sejam consistentes com a proposta da disciplina.

---

## 1. Objetivo Geral  
Implementar, **em C ou C++ com _pthreads_**, um jogo de console (ASCII) que demonstre na prática:  
- **Criação e gerenciamento de threads**  
- **Exclusão mútua** sobre recursos compartilhados  
- **Coordenação e comunicação** entre processos/threads (mutexes + variáveis de condição)  

O código servirá como estudo de caso de _multithreading_ na disciplina de Sistemas Operacionais.

---

## 2. Mecânica do Jogo  
- O jogador pilota um **helicóptero** e deve transportar **10 soldados** do lado esquerdo para uma plataforma no lado direito da tela.  
- Duas **baterias antiaéreas autopropulsadas** (Bateria 0 e Bateria 1) disparam foguetes aleatórios para derrubar o helicóptero.  
- **Condições de término**  
  1. Vitória: todos os soldados resgatados.  
  2. Derrota: helicóptero abatido por foguete.  
  3. Derrota: colisão do helicóptero com topo de tela ou qualquer obstáculo (solo, baterias, depósito, plataforma).  
- Cada bateria possui **munição finita** (`n` foguetes). Ao ficar sem munição:  
  - Desloca-se até um **depósito** à esquerda usando uma **ponte** (só uma bateria atravessa ou recarrega por vez).  
  - Tempo de recarga aleatório de 0,1 s a 0,5 s.  
- Três **níveis de dificuldade** (fácil, médio, difícil) alteram `n` e o tempo de recarga.  

---

## 3. Arquitetura de Threads  

| Thread | Função Principal | Recursos que Manipula |
|--------|-----------------|-----------------------|
| **Helicóptero** | Ler entrada do jogador, mover helicóptero, detectar colisões e realizar resgates | `pos_helicoptero`, `cont_soldados` |
| **Bateria 0** | Disparar foguetes, mover-se, gerenciar recarga e travessia da ponte | `estado_bat0`, `deposito`, `ponte` |
| **Bateria 1** | Idêntico à Bateria 0 | `estado_bat1`, `deposito`, `ponte` |
| **Foguetes** | Atualizar posições, criar/remover foguetes, detectar acertos | `lista_foguetes`, `pos_helicoptero` |
| **Interface** (opcional) | RedesENHAR a tela em ASCII a cada `refresh` ms | Somente leitura global |

> **Nota:** A interface em thread separada evita atrasos de renderização dentro das seções críticas do jogo.

---

## 4. Sincronização e Estruturas Críticas  

| Recurso Compartilhado | Mutex | Variável de Condição | Observações |
|-----------------------|-------|----------------------|-------------|
| Lista de foguetes             | `mtx_foguetes` | (opcional) `cv_foguetes` | Produtor (baterias) / consumidor (thread Foguetes) |
| Posição do helicóptero        | `mtx_heli`    | – | Leituras frequentes, escritas apenas pelo helicóptero |
| Estado de cada bateria        | `mtx_bat0`, `mtx_bat1` | – | Mun. restante, posição, flags de recarga |
| Contador de soldados          | `mtx_soldados` | – | Incrementado on-the-fly; pode usar operações atômicas |
| Depósito (recarregar)         | `mtx_deposito` | `cv_deposito` | Uma bateria por vez; outra aguarda |
| Ponte (ida e volta)           | `mtx_ponte`    | `cv_ponte`    | Exclusão mútua na travessia |

**Boas práticas**  
1. Manter ordem fixa de aquisição de locks para evitar *deadlock*.  
2. Conter a lógica de desenho fora das seções críticas.  
3. Usar **tempo mínimo de bloqueio** dentro dos mutexes.  

---

## 5. Diretórios Sugeridos  

├── src/
│ ├── main.c # inicialização e game-loop geral
│ ├── helicopter.c/h # lógica do helicóptero
│ ├── battery.c/h # lógica de Bateria 0 & 1 (parametrizada)
│ ├── rockets.c/h # gerenciamento de foguetes
│ ├── ui.c/h # renderização ASCII (opcional)
│ └── utils.h # srand, timers, helpers
├── include/ # headers públicos
└── AGENTS.md # (este arquivo) 👈


---

## 6. O que Esperamos da Codex  

1. **Gerar stubs** de arquivos acima, já com criação de threads e esqueleto de funções.  
2. Inserir **exemplos de uso** de `pthread_mutex_t` e `pthread_cond_t` seguindo os nomes definidos.  
3. Implementar **loop principal** que cria as threads, trata entrada de teclado (modo não-canônico em Linux), sincroniza estados e verifica condições de término.  
4. Fornecer **rotinas utilitárias** para sorteio de tempos de recarga e disparos aleatórios.  
5. Manter **comentários claros** explicando seções críticas, pontos de espera e sinais.  
6. Garantir **portabilidade** para Linux-x86-64 (gcc/clang, pthreads).  
7. Usar apenas **bibliotecas padrão** de C/C++ e `<pthread.h>`; nada de SDL, ncurses, etc.  
8. Priorizar **simplicidade e legibilidade**: esta é uma atividade didática.

---

## 7. Referência Rápida de Conquistas de Jogo

| Evento                     | Ação |
|----------------------------|------|
| Soldados resgatados == 10  | `WIN` |
| Foguete colide Helicóptero | `LOSE` |
| Helicóptero encosta no topo ou obstáculo | `LOSE` |

---

## 8. Conclusão  
Este documento consolida requisitos do trabalho, design de threads e regras de sincronização. **Codex**, siga estas diretrizes para gerar código que:  
- Demonstre corretamente conceitos de *threads* e sincronização,  
- Atenda às regras de negócio do jogo,  
- Seja completo o bastante para compilar e executar um protótipo funcional em console ASCII.  
Boa implementação! 🚁
