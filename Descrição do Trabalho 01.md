# Trabalho 1: Sincronização de Threads

O objetivo deste trabalho é que o estudante aplique os conceitos de **threads**, **exclusão mútua** e **coordenação de processos** por meio do projeto e implementação de um jogo, conforme segue.

---

## Descrição do Jogo

Um helicóptero, guiado pelo jogador, deve levar **10 soldados** da esquerda da tela para uma plataforma localizada à direita. Para impedir que o helicóptero cumpra a missão, há **duas baterias antiaéreas autopropulsadas inimigas** (bateria 0 e 1) que ficam continuamente disparando foguetes a esmo visando atingi-lo.

---

### O jogo termina numa das 3 situações abaixo:

1. O helicóptero de resgate cumprir a missão (**vitória do jogador**);
2. O helicóptero é abatido (**derrota do jogador**);
3. O helicóptero se choca contra algum obstáculo (**derrota do jogador**).

---

Quando os foguetes terminam, a bateria deve se deslocar para a esquerda, onde há um depósito onde ela é recarregada. O tempo de recarga demora de **1/10 s a 1/2 s**. Enquanto uma bateria está sendo recarregada, se a outra também necessita ser recarregada, ela deve aguardar.

Para uma bateria chegar ao depósito, há uma **ponte** na qual ela deve atravessar. Portanto, se a bateria 0 estiver atravessando a ponte, tanto no trajeto de ida quanto de volta, a bateria 1 deve aguardar e vice-versa.

---

O jogador pode movimentar o helicóptero para frente (seta →), para trás (seta ←), para cima (seta ↑) e para baixo (seta ↓). Se o usuário não pressiona nenhuma tecla, o helicóptero para. Se o helicóptero alcança o topo da tela (linha 0), ele explode. Se ele bate em algum obstáculo no solo (o próprio solo, alguma das baterias, o depósito ou a plataforma), ele também explode.

No anexo há uma versão tosca da interface usando apenas caracteres ASCII.

---

## Observações

- A capacidade de fabricação de foguetes é infinita, mas a bateria só consegue armazenar **n foguetes**;
- O jogo tem **3 níveis de dificuldade**:
    - **Fácil:** as baterias têm poucos foguetes e o tempo de recarga é alto;
    - **Médio:** as baterias têm uma quantidade média de foguetes e o tempo de recarga é médio;
    - **Difícil:** têm muitos foguetes e o tempo de recarga é baixo;
- O jogo **TEM** que ser implementado em **C ou C++** e usando **pthreads**.

---

## Grupos, Avaliação e Entrega

- O trabalho deverá ser realizado em **grupos de até 3 estudantes**.
- Os estudantes serão responsáveis por formar os grupos, e um dos componentes informará os nomes dos integrantes do grupo por e-mail.
- Um membro de cada grupo deverá submeter, via Moodle, um arquivo contendo o código-fonte em C ou C++ contendo a solução do trabalho.
- **Data/hora limite para o envio dos trabalhos:** **19/06/2025 às 23:55h**.
    - Trabalhos não postados no prazo terão um decréscimo de **20%** na nota.
- Após a data limite para entrega, os alunos deverão agendar a demonstração do trabalho com o professor.
- O professor irá avaliar não somente a **corretude** mas também o **desempenho** e a **clareza** da solução.
- Além disso, os estudantes serão avaliados pela **apresentação** e **entendimento** do trabalho.
- A **implementação** e **apresentação** valerão **50%** e **40%** da nota do trabalho, respectivamente.
- **10%** da nota será dedicado à definição das **threads necessárias** e da **estratégia de sincronização e exclusão mútua** que serão usadas.
- Os grupos deverão postar no Moodle um documento contendo tais informações até dia **01/06/2023, 23:55 horas**.
- Na apresentação, o professor questionará individualmente cada componente do grupo sobre aspectos das estratégias utilizadas e sobre aspectos de código da implementação.
