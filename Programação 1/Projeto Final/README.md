
# Projeto Final - Disciplina Programação 1 - THE BOYS 🌍🦸🏽👨🏽‍💻

## Descrição

Este projeto implementa uma simulação de um sistema de heróis que se deslocam entre bases, participam de missões e gerenciam suas ações em tempo real. O sistema utiliza eventos que são gerenciados por uma Lista de Eventos Futuros (LEF), e as operações seguem a política FIFO para a fila de espera dos heróis.

A simulação é realizada por meio de uma fila (FIFO) e uma lista de eventos futuros, garantindo que os eventos sejam processados na ordem de seus tempos de execução.

## Estruturas de Dados

### Fila (FIFO)

A fila é implementada utilizando uma lista ligada, onde os elementos são inseridos na ordem de chegada e removidos da frente da fila.

### Lista de Eventos Futuros (LEF)

A LEF é uma estrutura que armazena eventos ordenados por tempo, permitindo a simulação de um sistema de eventos. Cada evento possui um tipo (como CHEGA, ESPERA, DESISTE) e é inserido na LEF de acordo com seu tempo de ocorrência.

## Eventos

- **CHEGA (T, H, B):** Um herói chega a uma base e decide se vai esperar ou desistir, baseado na fila de espera da base.
- **ESPERA (T, H, B):** O herói entra na fila de espera da base.
- **DESISTE (T, H, B):** O herói desiste de esperar e escolhe outra base para viajar.
- **AVISA (T, B):** O porteiro da base verifica a fila de espera e permite a entrada de heróis.
- **ENTRA (T, H, B):** O herói entra na base e determina seu tempo de permanência.
- **SAI (T, H, B):** O herói sai da base e escolhe uma nova base para viajar.
- **VIAJA (T, H, D):** O herói viaja para uma nova base, e o tempo de viagem é calculado.
- **MISSAO (T, M):** Uma missão é disparada, e heróis são alocados para completar a missão.
- **FIM (T):** Encerra a simulação, apresentando as estatísticas dos heróis e missões.

