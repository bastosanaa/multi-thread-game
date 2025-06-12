
## Documentação de Threads, Papéis e Estratégias de Sincronização

### 1. Threads Utilizadas

No contexto do Trabalho 01 (jogo do helicóptero e baterias antiaéreas), serão criadas as seguintes threads principais:


| Thread | Papel |
| :-- | :-- |
| **Thread do Helicóptero** | Controla movimentação do helicóptero conforme entrada do jogador e verifica colisões/resgates |
| **Thread da Bateria 0** | Gerencia disparo de foguetes, movimentação, recarga e travessia da ponte da bateria 0 |
| **Thread da Bateria 1** | Idem, para a bateria 1 |
| **Thread dos Foguetes** | Gerencia movimentação de todos os foguetes ativos, detecta colisões, cria/remove foguetes |
| **Thread de Interface*** | Atualiza a interface gráfica/ASCII periodicamente (opcional, mas recomendada) |

\*A thread de interface é recomendada para desacoplar a lógica do jogo da exibição visual.

---

### 2. Papéis de Cada Thread

- **Thread do Helicóptero**
    - Captura entrada do jogador (teclas direcionais)
    - Move o helicóptero no cenário
    - Verifica colisões com foguetes, obstáculos, solo e topo da tela
    - Realiza o transporte de soldados e verifica condições de vitória/derrota
- **Thread da Bateria 0**
    - Dispara foguetes (solicita criação à thread dos foguetes)
    - Controla seu próprio movimento
    - Vai ao depósito para recarga quando sem munição
    - Gerencia travessia da ponte (respeitando exclusão mútua)
    - Aguarda caso depósito ou ponte estejam ocupados
- **Thread da Bateria 1**
    - Idêntica à da bateria 0, mas independente
    - Sincroniza acesso ao depósito e ponte com a bateria 0
- **Thread dos Foguetes**
    - Move todos os foguetes ativos na tela (atualização contínua)
    - Detecta colisão entre foguetes e helicóptero
    - Remove foguetes que saíram da tela ou colidiram
    - Recebe comandos das baterias para criar novos foguetes
- **Thread de Interface (Opcional)**
    - Atualiza a tela do jogo periodicamente
    - Garante que o jogador veja o estado mais recente dos objetos

---

### 3. Condições de Corrida e Sincronização

#### **Condições de Corrida Identificadas**

- **Acesso à lista de foguetes:**
    - As threads das baterias solicitam a criação de foguetes, enquanto a thread dos foguetes move, remove e verifica colisões.
    - **Risco:** Inserção/remoção simultânea pode corromper a estrutura de dados.
- **Acesso à posição do helicóptero:**
    - Tanto a thread dos foguetes quanto a do helicóptero precisam ler/modificar a posição.
    - **Risco:** Leituras e escritas simultâneas podem causar inconsistências.
- **Contador de soldados resgatados:**
    - Pode ser atualizado por mais de uma thread (por exemplo, ao detectar resgate ou colisão).
    - **Risco:** Incrementos simultâneos podem resultar em contagem incorreta.
- **Estado das baterias (munição, recarga, posição):**
    - As threads das baterias atualizam seu próprio estado, mas a thread dos foguetes pode precisar ler para criar foguetes.
    - **Risco:** Leitura/escrita concorrente pode gerar estado inválido.
- **Depósito de recarga:**
    - Apenas uma bateria pode recarregar por vez.
    - **Risco:** Duas baterias tentando recarregar simultaneamente.
- **Ponte:**
    - Apenas uma bateria pode atravessar a ponte por vez, em qualquer direção.
    - **Risco:** Colisão lógica se ambas tentarem atravessar ao mesmo tempo.

---

#### **Como Tratar Cada Situação**

- **Mutexes para Exclusão Mútua:**
    - Cada recurso compartilhado terá um mutex exclusivo:
        - Lista de foguetes: `mutex_foguetes`
        - Posição do helicóptero: `mutex_helicoptero`
        - Estado de cada bateria: `mutex_bateria_0`, `mutex_bateria_1`
        - Contador de soldados: `mutex_soldados`
        - Depósito: `mutex_deposito`
        - Ponte: `mutex_ponte`
- **Variáveis de Condição para Coordenação:**
    - Para depósito e ponte, usar variáveis de condição (`cond_deposito`, `cond_ponte`) para que uma bateria aguarde até o recurso estar livre, evitando espera ocupada.
- **Produtor-Consumidor para Foguetes:**
    - As threads das baterias atuam como produtoras (solicitam criação de foguetes), e a thread dos foguetes como consumidora (gerencia movimentação e remoção).
    - Sincronização via mutex e, se necessário, variáveis de condição para sinalizar novos foguetes.
- **Acesso Atômico a Contadores:**
    - Incrementos/decrementos protegidos por mutex ou usando operações atômicas, garantindo que a contagem de soldados e foguetes seja sempre correta.
- **Ordem de Locks e Boa Prática:**
    - Sempre adquirir os mutexes na mesma ordem para evitar deadlocks.
    - Minimizar o tempo dentro das seções críticas.
    - Separar lógica de interface da lógica do jogo para evitar bloqueios desnecessários.

---

### 4. Resumo Visual (ASCII)

```
+-----------------+         +-------------------+
| Thread          |         | Recursos Protegidos|
+-----------------+         +-------------------+
| Helicóptero     | <-----> | Posição, soldados |
| Bateria 0       | <-----> | Estado, depósito, ponte |
| Bateria 1       | <-----> | Estado, depósito, ponte |
| Foguetes        | <-----> | Lista de foguetes, helicóptero |
| Interface*      | <-----> | Estado geral (leitura) |
+-----------------+         +-------------------+
```


---

### 5. Estratégias Fundamentais

- **Mutexes**: Para garantir que apenas uma thread acesse um recurso compartilhado por vez[^9][^10][^11].
- **Variáveis de condição**: Para permitir que threads aguardem eventos (ex: depósito livre)[^10][^11].
- **Operações atômicas**: Para contadores simples, se possível[^6].
- **Separação de responsabilidades**: Cada thread tem papel claro e recursos bem definidos.

---

### 6. Conclusão

A divisão das threads e a estratégia de sincronização proposta garantem que o jogo funcione corretamente, sem condições de corrida ou inconsistências, mesmo com múltiplas entidades concorrendo por recursos limitados. O uso disciplinado de mutexes e variáveis de condição é fundamental para a robustez e desempenho do sistema, conforme exigido pelo enunciado do trabalho[^1].

<div style="text-align: center">⁂</div>

[^1]: Descricao-do-Trabalho-01.md

[^2]: https://stackoverflow.com/questions/44609174/c-pthreads-let-multiple-threads-synchronize-and-continue-together

[^3]: https://stackoverflow.com/questions/7469124/trying-to-implement-race-condition-using-pthread

[^4]: https://www.reddit.com/r/gamedev/comments/44fux4/multi_threading_in_game_development/

[^5]: https://stackoverflow.com/questions/60590421/how-to-properly-synchronize-threads-in-c-c-application

[^6]: https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/Multithreading/ThreadSafety/ThreadSafety.html

[^7]: https://github.com/adrientremblay/synchronization-pthread

[^8]: https://web.eecs.utk.edu/~jplank/plank/classes/cs360/360/notes/Thread-2-Race/lecture.html

[^9]: https://jorisvr.nl/article/cpp-thread-sync

[^10]: https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html

[^11]: https://sites.cs.ucsb.edu/~tyang/class/140s14/slides/Chapt4-pthreads-part1.pdf

[^12]: https://stackoverflow.com/questions/29813022/how-to-handle-synchronization-using-mutex-in-a-child-thread-in-c-programming

[^13]: https://softwareengineering.stackexchange.com/questions/382276/c-thread-architecture

[^14]: https://www.w3resource.com/c-programming/c-multithreading.php

[^15]: https://www.reddit.com/r/cprogramming/comments/1j86xhz/multithreading_in_c/

[^16]: https://kartikiyer.com/2019/06/16/synchronizing-multi-threaded-code-based-on-object-value/

[^17]: http://www.fragmentbuffer.com/docs/MultithreadingForGamedevStudents.pdf

[^18]: https://erikmcclure.com/blog/multithreading-problems-in-game-design/

[^19]: https://stackoverflow.com/questions/17024534/multithreading-and-synchronization

[^20]: https://docs.redhat.com/pt/documentation/red_hat_enterprise_linux_for_real_time/7/html/reference_guide/chap-thread_synchronization

[^21]: https://github.com/nana1904/game-of-life

[^22]: https://www.vkguide.dev/docs/extra-chapter/multithreading/

[^23]: https://www.eevblog.com/forum/programming/linux-how-to-synchronize-the-main-with-a-thread-in-c/

[^24]: https://www.cs.swarthmore.edu/~newhall/cs31/f19/Labs/lab09/

[^25]: https://gamedev.stackexchange.com/questions/31453/synchronization-between-game-logic-thread-and-rendering-thread

[^26]: https://www.youtube.com/watch?v=QuS9WSeW_7k

[^27]: https://stackoverflow.com/questions/60107010/race-conditions-problem-when-synchronizing-text-files-using-threads

[^28]: https://stackoverflow.com/questions/59113981/synchronization-between-threads-using-pthread-library

[^29]: https://github.com/zephyrproject-rtos/zephyr/issues/56163

[^30]: https://docs.oracle.com/cd/E19253-01/816-5137/gfwek/index.html

