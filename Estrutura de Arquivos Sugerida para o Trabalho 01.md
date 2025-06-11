<img src="https://r2cdn.perplexity.ai/pplx-full-logo-primary-dark%402x.png" class="logo" width="120"/>

## Estrutura de Arquivos Sugerida para o Trabalho 01

A estrutura de arquivos a seguir organiza o projeto para facilitar a manutenção, modularização e clareza, conforme as boas práticas para projetos em C com múltiplas threads, e considerando a separação das responsabilidades de cada thread e dos recursos compartilhados do jogo[^1].

```
trabalho01/
├── src/
│   ├── main.c                # Função principal, inicialização e encerramento do jogo
│   ├── helicopero.c          # Lógica da thread do helicóptero
│   ├── helicopero.h          # Header da thread do helicóptero
│   ├── bateria.c             # Lógica das threads das baterias antiaéreas
│   ├── bateria.h             # Header das baterias
│   ├── foguetes.c            # Lógica da thread dos foguetes (movimentação, colisão)
│   ├── foguetes.h            # Header dos foguetes
│   ├── interface.c           # Lógica da thread de interface/renderização (opcional)
│   ├── interface.h           # Header da interface
│   ├── recursos.c            # Controle de recursos compartilhados (depósito, ponte, soldados, mutexes)
│   ├── recursos.h            # Header dos recursos compartilhados
│   └── utils.c               # Funções auxiliares e utilitárias
│   └── utils.h               # Header das funções utilitárias
├── include/
│   └── config.h              # Definições globais (constantes, parâmetros de dificuldade, etc)
├── assets/
│   └── ascii_art.txt         # Arte ASCII opcional para interface
├── Makefile                  # Script de compilação
├── README.md                 # Instruções de uso e descrição do projeto
├── DOCUMENTACAO.md           # Documento de estratégia de threads e sincronização
└── Descricao-do-Trabalho-01.md # Enunciado oficial do trabalho (anexo)
```


---

### Função de Cada Arquivo

- **main.c**
Inicializa os recursos, cria as threads principais (helicóptero, baterias, foguetes, interface), faz a sincronização global e encerra o jogo.
- **helicopero.c/h**
Implementa a lógica da thread do helicóptero: leitura de teclado, movimentação, checagem de colisões e transporte de soldados.
- **bateria.c/h**
Implementa a lógica das threads das baterias: disparo, recarga, movimentação, travessia da ponte e sincronização com recursos.
- **foguetes.c/h**
Implementa a thread dos foguetes: movimentação, criação/remoção, checagem de colisão com o helicóptero.
- **interface.c/h**
(Opcional, mas recomendada) Atualiza a interface ASCII, exibe o estado do jogo e os contadores.
- **recursos.c/h**
Implementa e inicializa os recursos compartilhados: ponte, depósito, soldados, mutexes, variáveis de condição, e funções auxiliares de sincronização.
- **utils.c/h**
Funções auxiliares (ex: delay, geração de números aleatórios, manipulação de tempo, logging).
- **config.h**
Parâmetros de configuração do jogo (número de foguetes, tempo de recarga, dimensões da tela, etc).
- **ascii_art.txt**
Arte opcional para enriquecer a interface ASCII.
- **Makefile**
Automatiza a compilação do projeto.
- **README.md**
Explica como compilar, executar e jogar.
- **DOCUMENTACAO.md**
Documento detalhando as threads, papéis e estratégias de sincronização usadas no projeto.
- **Descricao-do-Trabalho-01.md**
Enunciado oficial do trabalho para consulta.

---

### Observações

- A separação em arquivos .c/.h facilita a manutenção e o trabalho em grupo.
- O uso de um diretório `include/` para headers globais ajuda a manter o projeto organizado.
- O arquivo `DOCUMENTACAO.md` é importante para a avaliação, pois detalha a estratégia de threads e sincronização, conforme exigido no edital do trabalho[^1].
- O Makefile garante compilação padronizada e fácil.

---

**Resumo:**
Esta estrutura modulariza cada componente do jogo (helicóptero, baterias, foguetes, interface e recursos compartilhados), facilita o trabalho colaborativo e atende às exigências do trabalho, além de seguir boas práticas de desenvolvimento em C com pthreads[^1].

<div style="text-align: center">⁂</div>

[^1]: Descricao-do-Trabalho-01.md

[^2]: https://www.inf.ufes.br/~rgomes/so_fichiers/aula16.pdf

[^3]: https://inf.ufes.br/~zegonc/material/Sistemas_Operacionais/Threads.pdf

[^4]: https://embarcados.com.br/threads-posix/

[^5]: https://www.dcc.fc.up.pt/~ricroc/aulas/1516/cp/apontamentos/slides_pthreads.pdf

[^6]: https://www.cin.ufpe.br/~jcbf/if677/2015-1/slides/Aula_05_Threads.pdf

[^7]: https://homepages.dcc.ufmg.br/~dorgival/cursos/so/tp2.pdf

[^8]: https://www.youtube.com/watch?v=BvAazf3iPU0

[^9]: https://www.di.ubi.pt/~operativos/praticos/html/9-threads.html

[^10]: https://www.lncc.br/~borges/ist/SO2/trabalhos/Pthreads.pdf

[^11]: https://pt.wikibooks.org/wiki/Programação_Paralela_em_Arquiteturas_Multi-Core/Programa%C3%A7%C3%A3o_em_Pthreads

