
## Estrutura de Arquivos Sugerida para o Trabalho

Com base na descrição detalhada do trabalho[^2] e na documentação do README[^1], segue uma proposta de estrutura de arquivos para organizar a implementação do jogo do helicóptero com pthreads, visando clareza, modularidade e facilidade de manutenção. Cada arquivo tem um papel bem definido, refletindo as responsabilidades das threads e os pontos críticos de sincronização.

```
trabalho01/
├── include/
│   ├── jogo.h              // Definições gerais do jogo, structs, enums, constantes
│   ├── helicoptero.h       // Interface da thread do helicóptero
│   ├── bateria.h           // Interface das threads das baterias
│   ├── foguete.h           // Interface da thread dos foguetes
│   ├── interface.h         // Interface da thread de interface (opcional)
│   ├── sincronizacao.h     // Definições de mutexes, variáveis de condição
├── src/
│   ├── main.c              // Função principal, inicialização e criação das threads
│   ├── jogo.c              // Implementação da lógica geral do jogo
│   ├── helicoptero.c       // Implementação da thread do helicóptero
│   ├── bateria.c           // Implementação das threads das baterias
│   ├── foguete.c           // Implementação da thread dos foguetes
│   ├── interface.c         // Implementação da thread de interface (opcional)
│   ├── sincronizacao.c     // Inicialização e destruição dos mecanismos de sincronização
├── assets/
│   └── mapas.txt           // (Opcional) ASCII art para cenários ou obstáculos
├── README.md               // Documentação e instruções de uso
├── Makefile                // Script de compilação
```


---

## Descrição dos Arquivos

**include/jogo.h**

- Estruturas de dados principais (helicóptero, baterias, foguetes, soldados, etc).
- Constantes de configuração (tamanho da tela, limites, número de soldados, etc).
- Enumerações para estados do jogo.

**include/helicoptero.h**

- Protótipo da função da thread do helicóptero.
- Funções auxiliares para movimentação e colisão.

**include/bateria.h**

- Protótipos das funções das threads das baterias.
- Funções para disparo, recarga e travessia da ponte.

**include/foguete.h**

- Protótipo da função da thread dos foguetes.
- Funções para criar, mover e remover foguetes.

**include/interface.h**

- Protótipo da função da thread de interface (opcional).
- Funções para atualizar a tela e exibir o estado do jogo.

**include/sincronizacao.h**

- Declaração dos mutexes e variáveis de condição globais.
- Funções para inicializar e destruir mecanismos de sincronização.

---

**src/main.c**

- Inicializa o estado do jogo.
- Inicializa mutexes e variáveis de condição.
- Cria as threads principais.
- Aguarda o término do jogo (join nas threads).
- Finaliza recursos.

**src/jogo.c**

- Implementação das regras gerais do jogo (condições de vitória/derrota, reinicialização, etc).

**src/helicoptero.c**

- Implementação da thread do helicóptero, captura de entrada do usuário, movimentação e verificação de colisões.

**src/bateria.c**

- Implementação das threads das baterias, lógica de disparo, recarga e travessia da ponte.

**src/foguete.c**

- Implementação da thread dos foguetes, movimentação, detecção de colisões e gerenciamento da lista de foguetes.

**src/interface.c**

- Atualização periódica da interface ASCII, desacoplada da lógica do jogo.

**src/sincronizacao.c**

- Inicialização e destruição de mutexes e variáveis de condição.

---

## Exemplo de main.c (resumido e comentado)

```c
#include "jogo.h"
#include "helicoptero.h"
#include "bateria.h"
#include "foguete.h"
#include "interface.h"
#include "sincronizacao.h"

int main() {
    inicializar_sincronizacao();
    inicializar_jogo();

    pthread_t th_helicop, th_bateria0, th_bateria1, th_foguetes, th_interface;

    pthread_create(&th_helicop, NULL, thread_helicoptero, NULL);
    pthread_create(&th_bateria0, NULL, thread_bateria, (void*)0);
    pthread_create(&th_bateria1, NULL, thread_bateria, (void*)1);
    pthread_create(&th_foguetes, NULL, thread_foguetes, NULL);
    pthread_create(&th_interface, NULL, thread_interface, NULL);

    // Aguarda o fim do jogo
    pthread_join(th_helicop, NULL);
    pthread_join(th_bateria0, NULL);
    pthread_join(th_bateria1, NULL);
    pthread_join(th_foguetes, NULL);
    pthread_join(th_interface, NULL);

    destruir_sincronizacao();
    return 0;
}
```

*Cada thread tem papel claro, e os recursos compartilhados são protegidos por mutexes e variáveis de condição, conforme detalhado no README[^1].*

---

## Recomendações

- Separe a lógica de sincronização (mutexes, variáveis de condição) em um módulo próprio para facilitar manutenção e evitar erros.
- Use comentários explicativos em cada função, especialmente nas seções críticas (lock/unlock).
- Utilize o Makefile para automatizar a compilação.
- Documente no README como compilar e rodar o jogo, além de explicar a função de cada thread e mecanismo de sincronização.

---

## Por que essa estrutura?

- **Modularidade:** Facilita testes, manutenção e futuras expansões.
- **Clareza:** Cada arquivo tem uma responsabilidade única, refletindo o papel de cada thread e recurso compartilhado[^1].
- **Facilidade de avaliação:** O professor conseguirá identificar rapidamente onde está cada parte da lógica e da sincronização, atendendo ao critério de clareza da solução[^2].

Se desejar, posso detalhar um exemplo de implementação para cada arquivo-chave ou fornecer um diagrama ASCII das relações entre threads e recursos.

<div style="text-align: center">⁂</div>

[^1]: README.md

[^2]: Descricao-do-Trabalho-01.md

