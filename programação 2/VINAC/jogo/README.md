
# Projeto TERONA - Um Jogo de Plataforma com Allegro 5

Este projeto é um jogo de plataforma 2D desenvolvido em C utilizando a biblioteca Allegro 5. O jogo apresenta um personagem jogável, plataformas, objetos móveis, inimigos (vilões), um sistema de vida, coleta de itens, diálogos e uma batalha contra um chefe.

## Visão Geral do Jogo

O objetivo principal do jogo é controlar o personagem "Megamente" através de diversos cenários de plataforma, enfrentando vilões, coletando itens e, por fim, derrotando o chefe final para alcançar a vitória.

### Funcionalidades Principais:

* **Sistema de Plataformas:** O jogo conta com múltiplos tipos de plataformas (pedra1, pedra2, pedra3) e plataformas móveis.
* **Movimentação do Jogador:** O personagem pode andar para a esquerda/direita, pular (inclusive pulo duplo), abaixar-se e disparar tiros.
* **Sistema de Tiro:**
    * **Tiro Normal:** Disparo padrão para a esquerda, direita ou para cima.
    * **Tiro Especial (SUPER):** Uma habilidade mais poderosa que causa mais dano, com um tempo de recarga (cooldown).
* **Inimigos (Vilões):**
    * Vilões que patrulham plataformas e atiram projéteis.
    * Eles possuem um ciclo de vida e animações de morte.
* **Chefe (Boss):**
    * Uma batalha contra um chefe com múltiplas fases: terrestre (andando, atirando, avançando) e aérea (subindo, voando, atacando, pausando).
    * O chefe possui um sistema de HP e estados de dano e morte progressivos.
    * Tiros do chefe são direcionados ao jogador e possuem um hitbox preciso.
* **Itens Coletáveis:**
    * **Item de Vida:** Restaura a vida do jogador.
    * **Item de Poder:** Concede ao jogador o tiro especial "SUPER".
* **Sistema de Diálogo:** Pequenas caixas de texto que aparecem na tela para fornecer informações ou narração.
* **Interface do Usuário (HUD):** Exibe a vida do jogador.
* **Morte e Reinício:** Se a vida do jogador chegar a zero ou cair abaixo do mapa, ele retorna ao último ponto de respawn, perdendo uma vida. Se todas as vidas forem perdidas, o jogo entra na tela de "Game Over".
* **Tela de Menu e Game Over:** Telas interativas para iniciar o jogo, reiniciar após a derrota ou sair.
* **Animações:** Personagens e itens possuem diversas animações (andar, pular, atirar, tomar dano, morrer, etc.).
* **Câmera Dinâmica:** A câmera segue o jogador horizontalmente.
* **Efeitos Visuais:** Pisca o jogador ao receber dano e exibe um alerta quando o "SUPER" está disponível.

## Estrutura do Projeto

O projeto é organizado nos seguintes arquivos:

* `main.c`: Contém a função `main` e o loop principal do jogo, gerenciando a máquina de estados (Menu, Jogando, Game Over).
* `codigo.c`: Contém a implementação das funções principais do jogo, incluindo a lógica dos estados do jogo (`executar_menu`, `executar_jogo`, `executar_game_over`), lógica de atualização de personagens, inimigos e colisões.
* `jogo.h`: Define as constantes, enums, estruturas de dados (Personagem, Vilao, Boss, Plataforma, Item, GameResources) e protótipos de todas as funções globais do jogo.
* `Bullet.h`, `Bullet.c`: Definem a estrutura de um projétil (`bullet`) e funções para movimentar e destruir projéteis.
* `Pistol.h`, `Pistol.c`: Definem a estrutura de uma "pistola" (`pistol`) que gerencia os projéteis disparados por um atirador (jogador ou vilão), e funções para criar tiros e gerenciar a pistola.
* `pistol_boss.h`, `pistol_boss.c`: Contêm funções de tiro específicas para o chefe, como tiros horizontais e tiros direcionados, utilizando a estrutura `pistol` e `bullet` existentes.

## Como Compilar e Rodar

Para compilar e rodar este jogo, você precisará ter o Allegro 5 instalado em seu sistema e um compilador C (como GCC).

### Pré-requisitos:

* **Allegro 5:** Certifique-se de ter o Allegro 5 e seus addons necessários instalados (image, primitives, font, ttf).
    * No Ubuntu/Debian: `sudo apt-get install liballegro5-dev liballegro5.2 liballegro-image5-dev liballegro-primitives5-dev liballegro-font5-dev liballegro-ttf5-dev`
    * No Windows, você pode baixar os pacotes pré-compilados do site oficial do Allegro.
* **Compilador C:** GCC é o mais comum.

### Passos para Compilar (Exemplo com GCC):

1.  **Navegue até a pasta do projeto** no seu terminal ou prompt de comando.
2.  **Compile os arquivos C** linkando com as bibliotecas Allegro 5. O comando pode variar ligeiramente dependendo da sua instalação do Allegro e do sistema operacional.

    Exemplo para Linux:
    ```bash
    gcc -o terona jogo.c codigo.c Pistol.c Bullet.c pistol_boss.c -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro -lm -std=c99
    ```
    * `terona`: É o nome do executável de saída.
    * `jogo.c codigo.c Pistol.c Bullet.c pistol_boss.c`: São todos os arquivos `.c` do seu projeto.
    * `-lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro`: Linka com os addons e a biblioteca principal do Allegro 5.
    * `-lm`: Linka com a biblioteca matemática (necessária para `sqrt` e `fmod`).
    * `-std=c99`: Garante a compatibilidade com o padrão C99 para certas funcionalidades.

    Para Windows (MinGW/MSYS2): Você pode precisar especificar o caminho para as bibliotecas Allegro e usar a flag `-mwindows` para evitar que a janela do console apareça.

### Passos para Rodar:

Após a compilação bem-sucedida, execute o jogo:

```bash
./terona
