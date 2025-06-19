
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

- **Mutexes**: Para garantir que apenas uma thread acesse um recurso compartilhado por vez.
- **Variáveis de condição**: Para permitir que threads aguardem eventos (ex: depósito livre).
- **Operações atômicas**: Para contadores simples, se possível.
- **Separação de responsabilidades**: Cada thread tem papel claro e recursos bem definidos.

---

### 6. Conclusão

A divisão das threads e a estratégia de sincronização proposta garantem que o jogo funcione corretamente, sem condições de corrida ou inconsistências, mesmo com múltiplas entidades concorrendo por recursos limitados. O uso disciplinado de mutexes e variáveis de condição é fundamental para a robustez e desempenho do sistema, conforme exigido pelo enunciado do trabalho[^1].

<div style="text-align: center">⁂</div>

[^1]: Descricao-do-Trabalho-01.md
