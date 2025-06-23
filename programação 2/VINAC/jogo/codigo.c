#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "Pistol.h" // Certifique-se que este arquivo existe e está correto
#include "jogo.h"   // Certifique-se que este arquivo existe e está correto, e que a struct Personagem foi atualizada nele

// --- Implementação das Funções da Máquina de Estados ---
bool esta_nCamera(float obj_x, float obj_width, float camera_x) {
    float camera_left = camera_x;
    float camera_right = camera_x + TELA_W;

    float obj_left = obj_x;
    float obj_right = obj_x + obj_width;

    // A condição de sobreposição horizontal
    return (obj_right > camera_left) && (obj_left < camera_right);
}

GameState executar_menu(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, GameResources *res) {
    int opcao_selecionada = 0;
    bool redraw = true; // Começa como true para desenhar o primeiro quadro

    al_flush_event_queue(queue);
    al_start_timer(timer); // Inicia o timer para este estado

    while (true) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        // --- Lógica de Eventos de Usuário ---
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_stop_timer(timer); // Para o timer antes de sair
            return ESTADO_SAIR;
        }
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                al_stop_timer(timer);
                return ESTADO_SAIR;
            }
            if (!ev.keyboard.repeat) {
                switch (ev.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:    opcao_selecionada = 0; break;
                    case ALLEGRO_KEY_DOWN:  opcao_selecionada = 1; break;
                    case ALLEGRO_KEY_ENTER:
                    case ALLEGRO_KEY_SPACE:
                        al_stop_timer(timer);
                        if (opcao_selecionada == 0) return ESTADO_JOGANDO;
                        if (opcao_selecionada == 1) return ESTADO_SAIR;
                        break;
                }
            }
        }

        // --- Lógica do Timer ---
        // A cada "tique" do relógio, marcamos que é hora de redesenhar.
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        }

        // --- Lógica de Desenho ---
        // Só desenha se for a hora certa E se não houver outros eventos na fila.
        if (redraw && al_is_event_queue_empty(queue)) {
            redraw = false;

            al_clear_to_color(al_map_rgb(0, 0, 0)); // Boa prática para limpar a tela
            al_draw_bitmap(res->fundo_menu, 0, 0, 0);
            desenhar_menu(res, opcao_selecionada);
            al_flip_display();
        }
    }
}

GameState executar_game_over(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, GameResources *res) {
    int opcao_selecionada = 0; // 0 para Reiniciar, 1 para Sair (ou Menu)
    bool redraw = true;

    al_flush_event_queue(queue);
    al_start_timer(timer);

    while (true) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        // --- Lógica de Eventos ---
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_stop_timer(timer);
            return ESTADO_SAIR;
        }

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (!ev.keyboard.repeat) {
                switch (ev.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        opcao_selecionada = 0; // Vai para a opção Reiniciar
                        break;
                    case ALLEGRO_KEY_DOWN:
                        opcao_selecionada = 1; // Vai para a opção Sair
                        break;
                    case ALLEGRO_KEY_ENTER:
                    case ALLEGRO_KEY_SPACE:
                        al_stop_timer(timer);
                        if (opcao_selecionada == 0) {
                            return ESTADO_JOGANDO; // Volta para o jogo (que será reiniciado)
                        } else {
                            return ESTADO_SAIR;    // Volta para o Menu Principal (melhor que fechar direto)
                        }
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        al_stop_timer(timer);
                        return ESTADO_MENU; // Pressionar ESC também volta para o Menu
                        break;
                }
            }
        }

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        }

        // --- Lógica de Desenho ---
        if (redraw && al_is_event_queue_empty(queue)) {
            redraw = false;

            // 1. Desenha o fundo de Game Over
            al_draw_scaled_bitmap(res->tela_go, 0, 0,
                                  al_get_bitmap_width(res->tela_go), al_get_bitmap_height(res->tela_go),
                                  0, 0, TELA_W, TELA_H, 0);

            // 2. Define a posição e tamanho dos botões
            float btn_w = 200.0f, btn_h = 80.0f;
            float btn_reiniciar_x = TELA_W / 2 - btn_w / 2, btn_reiniciar_y = TELA_H / 2 + 50;
            float btn_sair_x = TELA_W / 2 - btn_w / 2, btn_sair_y = TELA_H / 2 + 150;

            // 3. Desenha os botões na tela
            al_draw_scaled_bitmap(res->botao_reiniciar, 0, 0,
                                  al_get_bitmap_width(res->botao_reiniciar), al_get_bitmap_height(res->botao_reiniciar),
                                  btn_reiniciar_x, btn_reiniciar_y, btn_w, btn_h, 0);

            al_draw_scaled_bitmap(res->botao_sair, 0, 0,
                                  al_get_bitmap_width(res->botao_sair), al_get_bitmap_height(res->botao_sair),
                                  btn_sair_x, btn_sair_y, btn_w, btn_h, 0);

            // 4. Desenha o retângulo de seleção em volta do botão escolhido
            if (opcao_selecionada == 0) {
                al_draw_rectangle(btn_reiniciar_x - 5, btn_reiniciar_y - 5,
                                  btn_reiniciar_x + btn_w + 5, btn_reiniciar_y + btn_h + 5,
                                  al_map_rgb(255, 255, 0), 3.0);
            } else {
                al_draw_rectangle(btn_sair_x - 5, btn_sair_y - 5,
                                  btn_sair_x + btn_w + 5, btn_sair_y + btn_h + 5,
                                  al_map_rgb(255, 255, 0), 3.0);
            }

            al_flip_display();
        }
    }
}
bool proxima_pagina(char* pagina_buffer, int tamanho_buffer, char** ponteiro_leitura, int num_linhas) {
    // Se o ponteiro de leitura for nulo ou apontar para o fim da string, não há mais texto.
    if (!(*ponteiro_leitura) || **ponteiro_leitura == '\0') {
        pagina_buffer[0] = '\0'; // Limpa o buffer da página
        return false;
    }

    int linhas_contadas = 0;
    int i = 0;
    char* p = *ponteiro_leitura;

    // Copia o texto para o buffer da página
    while (*p != '\0' && i < tamanho_buffer - 1) {
        pagina_buffer[i] = *p;
        if (*p == '\n') {
            linhas_contadas++;
        }
        i++;
        p++;
        // Para quando atingimos o número desejado de linhas
        if (linhas_contadas >= num_linhas) {
            break;
        }
    }
    pagina_buffer[i] = '\0'; // Termina a string da página

    // Atualiza o ponteiro de leitura para a próxima chamada
    *ponteiro_leitura = p;
    return true;
}

// --- FUNÇÃO EXECUTAR_JOGO COMPLETA ---
GameState executar_jogo(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, GameResources *res) {
    // --- Objetos e Estados da Partida ---
    Plataforma plataformas[MAX_PLATAFORMAS];
    ObjetoMovel objetos_moveis[MAX_OBJETOS_MOVEIS];
    Personagem player;
    Vilao vetor_vilao[MAX_VILOES];
    float cooldown_super = 0.0f;
    long long frame_geral = 0;
    bool vitoria = false;


    // Zera a memória para evitar erros de ponteiros não inicializados
    memset(&player, 0, sizeof(Personagem));
    memset(vetor_vilao, 0, sizeof(Vilao) * MAX_VILOES);

    // --- Variáveis para o sistema de diálogo paginado ---
    char *texto_completo = NULL;      // Armazena o diálogo inteiro do arquivo
    char *ponteiro_leitura = NULL;    // Aponta para onde estamos na leitura do texto_completo
    char texto_pagina_atual[512];   // Buffer para armazenar apenas a página atual
    texto_pagina_atual[0] = '\0';   // Inicializa o buffer como vazio
    bool dialogo_ativo = false;           // Controla se a caixa de diálogo está visível
    bool jogo_pausado = false;            // Controla se a lógica do jogo está parada

    // --- Configuração do Nível ---
    float escala_padrao_pedra_p2_p4 = 0.5f;
    plataformas[0]  = (Plataforma){res->pedra_sprites[0], 200, 500, al_get_bitmap_width(res->pedra_sprites[0]) * 1.0f, al_get_bitmap_height(res->pedra_sprites[0]) * 1.0f};
    plataformas[1]  = (Plataforma){res->pedra_sprites[1], 1000, 450, al_get_bitmap_width(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4};
    plataformas[2]  = (Plataforma){res->pedra_sprites[1], 1400, 500, al_get_bitmap_width(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4};
    plataformas[3]  = (Plataforma){res->pedra_sprites[2], 1800, 600, al_get_bitmap_width(res->pedra_sprites[2]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[2]) * escala_padrao_pedra_p2_p4};
    plataformas[4]  = (Plataforma){res->pedra_sprites[1], 2300, 500, al_get_bitmap_width(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4};
    plataformas[5]  = (Plataforma){res->pedra_sprites[2], 2600, 425, al_get_bitmap_width(res->pedra_sprites[2]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[2]) * escala_padrao_pedra_p2_p4};
    plataformas[6]  = (Plataforma){res->pedra_sprites[1], 3000, 270, al_get_bitmap_width(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4};
    plataformas[7]  = (Plataforma){res->pedra_sprites[1], 3000, 570, al_get_bitmap_width(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4};
    plataformas[8]  = (Plataforma){res->pedra_sprites[0], 3500, 420, al_get_bitmap_width(res->pedra_sprites[0]) * 1.0f, al_get_bitmap_height(res->pedra_sprites[0]) * 1.0f};
    plataformas[9]  = (Plataforma){res->pedra_sprites[2], 4300, 500, al_get_bitmap_width(res->pedra_sprites[2]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[2]) * escala_padrao_pedra_p2_p4};
    plataformas[10] = (Plataforma){res->pedra_sprites[1], 4700, 480, al_get_bitmap_width(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4};
    plataformas[11] = (Plataforma){res->pedra_sprites[2], 5200, 480, al_get_bitmap_width(res->pedra_sprites[2]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[2]) * escala_padrao_pedra_p2_p4};
    plataformas[12] = (Plataforma){res->pedra_sprites[0], 5800, 490, al_get_bitmap_width(res->pedra_sprites[0]) * 1.0f, al_get_bitmap_height(res->pedra_sprites[0]) * 1.0f};
    plataformas[13] = (Plataforma){res->pedra_sprites[0], 7200, 630, al_get_bitmap_width(res->pedra_sprites[0]) * 1.0f, al_get_bitmap_height(res->pedra_sprites[0]) * 1.0f};
    plataformas[14] = (Plataforma){res->pedra_sprites[1], 8000, 600, al_get_bitmap_width(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4};
    plataformas[15] = (Plataforma){res->pedra_sprites[2], 8290, 550, al_get_bitmap_width(res->pedra_sprites[2]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[2]) * escala_padrao_pedra_p2_p4};
    plataformas[16] = (Plataforma){res->pedra_sprites[2], 8590, 400, al_get_bitmap_width(res->pedra_sprites[2]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[2]) * escala_padrao_pedra_p2_p4};
    plataformas[17] = (Plataforma){res->pedra_sprites[1], 8890, 350, al_get_bitmap_width(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4};
    plataformas[18] = (Plataforma){res->pedra_sprites[0], 9200, 350, al_get_bitmap_width(res->pedra_sprites[0]) * 1.0f, al_get_bitmap_height(res->pedra_sprites[0]) * 1.0f};
    plataformas[19] = (Plataforma){res->pedra_sprites[2], 10000, 400, al_get_bitmap_width(res->pedra_sprites[2]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[2]) * escala_padrao_pedra_p2_p4};
    plataformas[20] = (Plataforma){res->pedra_sprites[0], 10300, 400, al_get_bitmap_width(res->pedra_sprites[0]) * 1.0f, al_get_bitmap_height(res->pedra_sprites[0]) * 1.0f};
    plataformas[21] = (Plataforma){res->pedra_sprites[1], 11100, 480, al_get_bitmap_width(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4};
    plataformas[22] = (Plataforma){res->pedra_sprites[0], 11500, 480, al_get_bitmap_width(res->pedra_sprites[0]) * 1.0f, al_get_bitmap_height(res->pedra_sprites[0]) * 1.0f};
    plataformas[23] = (Plataforma){res->pedra_sprites[1], 12300, 500, al_get_bitmap_width(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4};
    plataformas[24] = (Plataforma){res->pedra_sprites[1], 12700, 480, al_get_bitmap_width(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4};
    plataformas[25] = (Plataforma){res->pedra_sprites[1], 13100, 410, al_get_bitmap_width(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4};
    plataformas[26] = (Plataforma){res->pedra_sprites[1], 13300, 390, al_get_bitmap_width(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4};
    plataformas[27] = (Plataforma){res->pedra_sprites[1], 14700, 390, al_get_bitmap_width(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->pedra_sprites[1]) * escala_padrao_pedra_p2_p4};
    plataformas[28] = (Plataforma){res->pedra_sprites[0], 15000, TELA_H - al_get_bitmap_height(res->pedra_sprites[0]), TELA_W, al_get_bitmap_height(res->pedra_sprites[0])};


    objetos_moveis[0] = (ObjetoMovel){res->obj_movel_sprites[0], 6400.0f, plataformas[10].y - al_get_bitmap_height(res->obj_movel_sprites[0]), al_get_bitmap_width(res->obj_movel_sprites[0]), al_get_bitmap_height(res->obj_movel_sprites[0]), 6400.0f, 7000.0f, 3.0f, 1};
    objetos_moveis[1] = (ObjetoMovel){res->obj_movel_sprites[1], 13500.0f, plataformas[26].y, al_get_bitmap_width(res->obj_movel_sprites[1]) * escala_padrao_pedra_p2_p4, al_get_bitmap_height(res->obj_movel_sprites[1]) * escala_padrao_pedra_p2_p4, 13500.0f, 14500.0f, 3.0f, 1};

    reiniciar_jogo(&player, plataformas, res);
    for(int i = 0; i < MAX_VILOES; i++) {
        reiniciar_vilao(&vetor_vilao[i], plataformas, i, res);
    }
    Boss boss;
    inicializar_boss(&boss, &plataformas[PLATAFORMA_BOSS_IDX], res);

    Item itens[MAX_ITENS];
   // --- Inicialização do Item 0 (Vida) ---
    itens[0].ativo = true;
    itens[0].tipo = TIPO_ITEM_VIDA;
    itens[0].sprite = res->sprites_item_vida; // Aponta para os sprites de vida
    itens[0].num_frames = FRAMES_ITEM_VIDA;
    itens[0].intervalo_animacao = INTERVALO_ANIMACAO_ITEM;
    itens[0].frame_atual = 0;
    itens[0].frame_contador = 0;
    itens[0].width = al_get_bitmap_width(itens[0].sprite[0]) * ESCALA_ITEM_VIDA;
    itens[0].height = al_get_bitmap_height(itens[0].sprite[0]) * ESCALA_ITEM_VIDA;
    Plataforma p_vida = plataformas[PLATAFORMA_ITEM_VIDA];
    itens[0].x = p_vida.x + (p_vida.width / 2) - (itens[0].width / 2);
    itens[0].y = p_vida.y + AJUSTE_VERTICAL_PEDRA - itens[0].height - ALTURA_FLUTUACAO_ITEM;


    // --- Inicialização do Item 1 (Poder) ---
    itens[1].ativo = true;
    itens[1].tipo = TIPO_ITEM_PODER;
    itens[1].sprite = res->sprites_item_poder; // Aponta para os sprites de poder
    itens[1].num_frames = FRAMES_ITEM_VIDA;
    itens[1].intervalo_animacao = INTERVALO_ANIMACAO_ITEM;
    itens[1].frame_atual = 0;
    itens[1].frame_contador = 0;
    itens[1].width = al_get_bitmap_width(itens[1].sprite[0]) * 0.35; // Escala do item de poder
    itens[1].height = al_get_bitmap_height(itens[1].sprite[0]) * 0.35;
    Plataforma p_poder = plataformas[PLATAFORMA_ITEM_PODER];
    itens[1].x = p_poder.x + (p_poder.width / 2) - (itens[1].width / 2);
    itens[1].y = p_poder.y + AJUSTE_VERTICAL_PEDRA - itens[1].height - ALTURA_FLUTUACAO_ITEM;;

    int ultima_plataforma_id = 0;
    float camera_x = 0;
    bool tecla_pulo_pressionada_frame_anterior = false;
    bool redraw = true;

    al_flush_event_queue(queue);
    al_start_timer(timer);

    while (true) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            if (texto_completo) free(texto_completo);
            al_stop_timer(timer);
            return ESTADO_SAIR;
        }

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (vitoria) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    if (texto_completo) free(texto_completo);
                    return ESTADO_MENU;
                }
            }
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                if (dialogo_ativo) {
                    if (!proxima_pagina(texto_pagina_atual, sizeof(texto_pagina_atual), &ponteiro_leitura, 3)) {
                        dialogo_ativo = false;
                        jogo_pausado = false;
                    }
                }
                else {
                    if (!player.esta_animando_poder) {
                        jogo_pausado = !jogo_pausado;
                    }
                }
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                if (!player.esta_animando_poder) {
                    jogo_pausado = !jogo_pausado;
                }
            }
        }

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            frame_geral++;

            if (player.esta_animando_poder) {
                player.contador_frame_poder++;
                player.frame_animacao_poder = player.contador_frame_poder / INTERVALO_ANIMACAO_PODER;

                Plataforma p_item = plataformas[PLATAFORMA_ITEM_PODER];
                float base_y = (p_item.y + AJUSTE_VERTICAL_PEDRA) - player.char_height;
                float mid_point = (FRAMES_PODER - 1) / 2.0f;
                float current_progress = (mid_point - fabs(player.frame_animacao_poder - mid_point)) / mid_point;
                player.y = base_y - (current_progress * ALTURA_FLUTUACAO_PODER);

                if (player.frame_animacao_poder >= FRAMES_PODER) {
                    player.esta_animando_poder = false;
                    player.estado = PARADO;
                    player.y = base_y;

                    if (texto_completo) free(texto_completo);
                    texto_completo = carregar_texto_de_arquivo("./textos/test.txt");
                    if (texto_completo) {
                        ponteiro_leitura = texto_completo;
                        proxima_pagina(texto_pagina_atual, sizeof(texto_pagina_atual), &ponteiro_leitura, 3);
                        dialogo_ativo = true;
                    }
                }
            }
            else if (!jogo_pausado && !vitoria) {

                ALLEGRO_KEYBOARD_STATE ks; al_get_keyboard_state(&ks);
                ALLEGRO_MOUSE_STATE mouse_state; al_get_mouse_state(&mouse_state);

                float old_y = player.y;

                if (player.timer_invencivel > 0) {
                    player.timer_invencivel--;
                }

                if (cooldown_super > 0) {
                    cooldown_super -= 1.0 / FPS;
                }
                if (cooldown_super < 0) {
                    cooldown_super = 0;
                }

                for (int i = 0; i < MAX_ITENS; i++) {
                    if (itens[i].ativo) {
                        itens[i].frame_contador++;
                        if (itens[i].frame_contador >= itens[i].num_frames * itens[i].intervalo_animacao) {
                            itens[i].frame_contador = 0;
                        }
                        itens[i].frame_atual = itens[i].frame_contador / itens[i].intervalo_animacao;
                    }
                }

                for (int i = 0; i < MAX_OBJETOS_MOVEIS; i++) {
                    float old_obj_x = objetos_moveis[i].x;
                    objetos_moveis[i].x += objetos_moveis[i].move_speed * objetos_moveis[i].move_direction;
                    if (objetos_moveis[i].x >= objetos_moveis[i].end_x || objetos_moveis[i].x <= objetos_moveis[i].start_x) {
                        objetos_moveis[i].move_direction *= -1;
                    }
                    if (ultima_plataforma_id == MAX_PLATAFORMAS + i && player.pulos_restantes == MAX_PULOS) {
                        player.x += objetos_moveis[i].x - old_obj_x;
                    }
                }
                if (player.gun->timer > 0) player.gun->timer--;
                if (player.especial_gun && player.especial_gun->timer > 0) player.especial_gun->timer--;

                for(int i = 0; i < MAX_VILOES; i++) { atualizar_vilao(&vetor_vilao[i], &player, ultima_plataforma_id); }

                atualizar_boss_batalha(&boss, &player, &plataformas[PLATAFORMA_BOSS_IDX], ultima_plataforma_id, &ks);

                player.vel_x = 0;
                bool andando_neste_frame = false;
                if (al_key_down(&ks, ALLEGRO_KEY_RIGHT) || al_key_down(&ks, ALLEGRO_KEY_D)) { player.vel_x = VELOCIDADE_JOGADOR; player.virado_esquerda = false; andando_neste_frame = true; }
                else if (al_key_down(&ks, ALLEGRO_KEY_LEFT) || al_key_down(&ks, ALLEGRO_KEY_A)) { player.vel_x = -VELOCIDADE_JOGADOR; player.virado_esquerda = true; andando_neste_frame = true; }

                bool tecla_pulo_atual = al_key_down(&ks, ALLEGRO_KEY_UP) || al_key_down(&ks, ALLEGRO_KEY_W);
                if (tecla_pulo_atual && !tecla_pulo_pressionada_frame_anterior && player.pulos_restantes > 0) { player.estado = PULANDO; player.vel_y = FORCA_PULO; player.pulos_restantes--; player.frame_atual = 0; }
                tecla_pulo_pressionada_frame_anterior = tecla_pulo_atual;

                if (player.estado != PULANDO) {
                    if (al_key_down(&ks, ALLEGRO_KEY_DOWN) || al_key_down(&ks, ALLEGRO_KEY_S)) { player.estado = ABAIXANDO; }
                    else { player.estado = andando_neste_frame ? ANDANDO : PARADO; }
                }

                if ((mouse_state.buttons & 1 || al_key_down(&ks, ALLEGRO_KEY_SPACE))) {
                    if (player.gun->timer == 0) {
                        if (player.estado != PULANDO) {
                            if (player.estado != ANDANDO_ATIRANDO && player.estado != PARADO_ATIRANDO) player.frame_atual = 0;
                            player.estado = andando_neste_frame ? ANDANDO_ATIRANDO : PARADO_ATIRANDO;
                        }

                        DirecaoTiro direcao_atual;
                        float start_x, start_y;

                        if (al_key_down(&ks, ALLEGRO_KEY_UP) || al_key_down(&ks, ALLEGRO_KEY_W)) {
                            direcao_atual = DIR_CIMA;
                            if (player.estado != PULANDO) player.estado = PARADO_ATIRANDO_CIMA;
                            float largura_final_tiro = al_get_bitmap_width(res->tiro_player) * 0.2f;
                            start_x = player.x + (player.char_width / 2.0f) - (largura_final_tiro / 2.0f);
                            start_y = player.y - 100;
                        }
                        else {
                            if (player.virado_esquerda) {
                                direcao_atual = DIR_ESQUERDA;
                                start_x = player.x - 100;
                            } else {
                                direcao_atual = DIR_DIREITA;
                                start_x = player.x + player.char_width;
                            }
                            start_y = player.y + (player.char_height * 0.2f);
                        }

                        player.gun->shots = pistol_shot(start_x, start_y, direcao_atual, player.gun);
                        player.gun->timer = PISTOL_COOLDOWN;
                    }
                }
                else if ((mouse_state.buttons & 2 || al_key_down(&ks,ALLEGRO_KEY_TAB)) && player.tem_poder_especial && cooldown_super <= 0) {
                    if (player.especial_gun->timer == 0) {
                        if (player.estado != PULANDO) player.estado = PARADO_ATIRANDO;

                        DirecaoTiro direcao_atual;
                        float start_x, start_y;

                        if (al_key_down(&ks, ALLEGRO_KEY_UP) || al_key_down(&ks, ALLEGRO_KEY_W)) {
                            direcao_atual = DIR_CIMA;
                            start_x = player.x + (player.virado_esquerda ? -100.0f : player.char_width - 30.0f);
                            start_y = player.y - 100;
                        }
                        else {
                            if (player.virado_esquerda) {
                                direcao_atual = DIR_ESQUERDA;
                                start_x = player.x - 100;
                            } else {
                                direcao_atual = DIR_DIREITA;
                                start_x = player.x + (player.virado_esquerda ? -100.0f : player.char_width - 30.0f);;
                            }
                            start_y = player.y + (player.char_height * 0.35f);
                        }

                        player.especial_gun->shots = pistol_shot(start_x, start_y, direcao_atual, player.especial_gun);
                        player.especial_gun->timer = PISTOL_COOLDOWN;
                        cooldown_super = COOLDOWN_SUPER_SEGUNDOS;
                    }
                }

                player.vel_y += GRAVIDADE;
                player.x += player.vel_x;
                player.y += player.vel_y;

                bool em_terra = false;
                if(player.vel_y >= 0) {
                    for (int i = 0; i < MAX_PLATAFORMAS; i++) {
                        float y_chao = plataformas[i].y + AJUSTE_VERTICAL_PEDRA;
                        if ((player.y + player.char_height >= y_chao) && (old_y + player.char_height <= y_chao) && (player.x < plataformas[i].x + plataformas[i].width) && (player.x + player.char_width > plataformas[i].x)) {
                            player.y = y_chao - player.char_height;
                            em_terra = true;
                            ultima_plataforma_id = i;
                            break;
                        }
                    }
                    if (!em_terra) {
                        for (int i = 0; i < MAX_OBJETOS_MOVEIS; i++) {
                            if ((old_y + player.char_height <= objetos_moveis[i].y + 1) && colisao(player.x, player.y, player.char_width, player.char_height, objetos_moveis[i].x, objetos_moveis[i].y, objetos_moveis[i].width, objetos_moveis[i].height)) {
                                player.y = objetos_moveis[i].y - player.char_height;
                                em_terra = true;
                                ultima_plataforma_id = MAX_PLATAFORMAS + i;
                                break;
                            }
                        }
                    }
                }

                if(em_terra) {
                    player.vel_y = 0;
                    player.pulos_restantes = MAX_PULOS;
                    if(player.estado == PULANDO) player.estado = PARADO;
                    if (ultima_plataforma_id < MAX_PLATAFORMAS) { player.respawn_x = player.x; player.respawn_y = player.y; }
                } else {
                    if(player.pulos_restantes == MAX_PULOS) player.pulos_restantes--;
                    if(player.estado != PULANDO && player.estado != ANDANDO_ATIRANDO && player.estado != PARADO_ATIRANDO && player.estado != PARADO_ATIRANDO_CIMA) player.estado = PULANDO;
                }

                bullet_move(player.gun->shots);
                bullet_move(player.especial_gun->shots);
                // Movimenta os tiros do boss também
                bullet_move(boss.gun->shots);

                // Colisão do jogador com itens (usa a hitbox geral do jogador)
                float player_hitbox_x = player.x + player.hitbox_x_offset;
                float player_hitbox_y = player.y + player.hitbox_y_offset;

                for (int i = 0; i < MAX_ITENS; i++) {
                    if (itens[i].ativo && colisao(player_hitbox_x, player_hitbox_y, player.hitbox_width,
                    player.hitbox_height, itens[i].x, itens[i].y, itens[i].width, itens[i].height)) {

                       switch (itens[i].tipo) {
                            case TIPO_ITEM_VIDA:
                                itens[i].ativo = false;
                                player.vidas = MAX_VIDAS;
                                if (texto_completo) free(texto_completo);
                                texto_completo = carregar_texto_de_arquivo("./textos/dialogo.txt");
                                if (texto_completo) {
                                    ponteiro_leitura = texto_completo;
                                    dialogo_ativo = true;
                                    jogo_pausado = true;
                                    proxima_pagina(texto_pagina_atual, sizeof(texto_pagina_atual), &ponteiro_leitura, 3);
                                }
                                break;
                            case TIPO_ITEM_PODER:
                                itens[i].ativo = false;
                                player.tem_poder_especial = true;
                                Plataforma p_item = plataformas[PLATAFORMA_ITEM_PODER];
                                player.x = (p_item.x + p_item.width / 2.0f) - (player.char_width / 2.0f);
                                player.y = (p_item.y + AJUSTE_VERTICAL_PEDRA) - player.char_height;
                                player.vel_y = 0;
                                player.esta_animando_poder = true;
                                player.frame_animacao_poder = 0;
                                player.contador_frame_poder = 0;
                                jogo_pausado = true;
                                break;
                        }
                    }
                }

                // Colisão dos tiros do jogador com VILÕES (Normal e Especial)
                for (int i = 0; i < MAX_VILOES; i++) {
                    if (vetor_vilao[i].ativo && vetor_vilao[i].estado != V_MORRENDO) {
                        if (esta_nCamera(vetor_vilao[i].x, vetor_vilao[i].width, camera_x)) {
                            // Tiros normais do player
                            for (bullet *b = player.gun->shots, *prev = NULL; b != NULL; ) {
                                float tiro_player_w = al_get_bitmap_width(res->tiro_player) * 0.2f;
                                float tiro_player_h = al_get_bitmap_height(res->tiro_player) * 0.2f;

                                if (colisao(b->x, b->y, tiro_player_w, tiro_player_h,
                                            vetor_vilao[i].x, vetor_vilao[i].y, vetor_vilao[i].width, vetor_vilao[i].height)) {
                                    vetor_vilao[i].vidas--;
                                    if (vetor_vilao[i].vidas <= 0) { vetor_vilao[i].estado = V_MORRENDO; vetor_vilao[i].frame_atual = 0; }
                                    bullet *temp = b;
                                    b = (bullet*)b->next;
                                    if (prev == NULL) player.gun->shots = (bullet*)temp->next;
                                    else prev->next = temp->next;
                                    bullet_destroy(temp);
                                } else { prev = b; b = (bullet*)b->next; }
                            }

                            // Tiros especiais do player
                            for (bullet *b = player.especial_gun->shots, *prev = NULL; b != NULL; ) {
                                float tiro_especial_w = al_get_bitmap_width(res->tiro_especial) * 2.0f;
                                float tiro_especial_h = al_get_bitmap_height(res->tiro_especial) * 2.0f;

                                if (colisao(b->x, b->y, tiro_especial_w, tiro_especial_h,
                                            vetor_vilao[i].x, vetor_vilao[i].y, vetor_vilao[i].width, vetor_vilao[i].height)) {
                                    vetor_vilao[i].vidas -= 2;
                                    if (vetor_vilao[i].vidas <= 0) { vetor_vilao[i].estado = V_MORRENDO; vetor_vilao[i].frame_atual = 0; }
                                    bullet *temp = b; b = (bullet*)b->next;
                                    if (prev == NULL) player.especial_gun->shots = (bullet*)temp->next;
                                    else prev->next = temp->next;
                                    bullet_destroy(temp);
                                } else { prev = b; b = (bullet*)b->next; }
                            }
                        }
                    }
                }

                // Colisão dos TIROS DOS VILÕES com o JOGADOR
                // Usa a nova hitbox de projétil do jogador
                float player_proj_hitbox_x = player.x + player.projectile_hitbox_x_offset;
                float player_proj_hitbox_y = player.y + player.projectile_hitbox_y_offset;

                // Define as dimensões do tiro do vilão baseado na escala de desenho (2.0f)
                float tiro_vilao_w = al_get_bitmap_width(res->tiro_vilao) * 0.5f;
                float tiro_vilao_h = al_get_bitmap_height(res->tiro_vilao) * 0.5f;

                for (int i = 0; i < MAX_VILOES; i++) {
                    for (bullet *b = vetor_vilao[i].gun->shots, *prev = NULL; b != NULL; ) {
                        // Passa as dimensões corretas do tiro do vilão para a função colisao
                        if (colisao(b->x, b->y, tiro_vilao_w, tiro_vilao_h,
                                    player_proj_hitbox_x, player_proj_hitbox_y,
                                    player.projectile_hitbox_width, player.projectile_hitbox_height)) {
                            if (player.timer_invencivel <= 0) { // Só causa dano se não estiver invencível
                                player.vidas--;
                                player.timer_invencivel = 90; // Deixa o jogador invencível por 1.5 segundos (90 frames)
                            }
                            // Destrói o projétil assim que ele colide (seja invencível ou não)
                            bullet *temp = b;
                            b = (bullet*)b->next;
                            if (prev == NULL) vetor_vilao[i].gun->shots = (bullet*)temp->next;
                            else prev->next = temp->next;
                            bullet_destroy(temp);
                            break; // Um tiro colidiu, saia deste loop para o próximo vilão
                        } else {
                            prev = b;
                            b = (bullet*)b->next;
                        }
                    }
                }

                // Colisão dos tiros do JOGADOR com o BOSS
                if (boss.ativo && boss.timer_dano <= 0 && boss.estado != B_MORRENDO_IMPACTO && boss.estado != B_MORRENDO_CAINDO && boss.estado != B_MORRENDO_NO_CHAO) {
                    // Loop para o tiro NORMAL do jogador
                    for (bullet *b = player.gun->shots, *prev = NULL; b != NULL; ) {
                        float tiro_player_w = al_get_bitmap_width(res->tiro_player) * 0.2f;
                        float tiro_player_h = al_get_bitmap_height(res->tiro_player) * 0.2f;

                        if (colisao(b->x, b->y, tiro_player_w, tiro_player_h, boss.x, boss.y, boss.width, boss.height)) {
                            boss.hp--;
                            boss.estado = B_TOMANDO_DANO;
                            boss.frame_atual = 0;
                            boss.timer_dano = 30; // 0.5s de invulnerabilidade
                            if (boss.hp <= 0) { boss.estado = B_MORRENDO_IMPACTO; boss.frame_atual = 0; }

                            // Destrói o projétil
                            bullet *temp = b;
                            b = (bullet*)b->next;
                            if (prev == NULL) player.gun->shots = (bullet*)temp->next;
                            else prev->next = temp->next;
                            bullet_destroy(temp);
                            continue; // Pula para o próximo projétil
                        }
                        prev = b;
                        b = (bullet*)b->next;
                    }

                    // Loop para o tiro ESPECIAL do jogador
                    for (bullet *b = player.especial_gun->shots, *prev = NULL; b != NULL; ) {
                        float tiro_especial_w = al_get_bitmap_width(res->tiro_especial) * 2.0f;
                        float tiro_especial_h = al_get_bitmap_height(res->tiro_especial) * 2.0f;

                        if (colisao(b->x, b->y, tiro_especial_w, tiro_especial_h, boss.x, boss.y, boss.width, boss.height)) {
                            boss.hp -= 2; // Causa mais dano
                            boss.estado = B_TOMANDO_DANO;
                            boss.frame_atual = 0;
                            boss.timer_dano = 30;
                            if (boss.hp <= 0) { boss.hp = 0; boss.estado = B_MORRENDO_IMPACTO; boss.frame_atual = 0; }

                            // Destrói o projétil
                            bullet *temp = b;
                            b = (bullet*)b->next;
                            if (prev == NULL) player.especial_gun->shots = (bullet*)temp->next;
                            else prev->next = temp->next;
                            bullet_destroy(temp);
                            continue;
                        }
                        prev = b;
                        b = (bullet*)b->next;
                    }
                }

                // NOVO BLOCO: VERIFICA COLISÃO DO TIRO DO BOSS COM O JOGADOR ---
                // Só causa dano se o jogador não estiver invencível
                if (player.timer_invencivel <= 0) {
                    // Usa a hitbox de projétil do jogador
                    float player_proj_hitbox_x = player.x + player.projectile_hitbox_x_offset;
                    float player_proj_hitbox_y = player.y + player.projectile_hitbox_y_offset;

                    // Define as dimensões do tiro do boss baseado na escala de desenho (2.5f)
                    float boss_tiro_w = al_get_bitmap_width(res->tiro_vilao) * 2.5f;
                    float boss_tiro_h = al_get_bitmap_height(res->tiro_vilao) * 2.5f;

                    for (bullet *b = boss.gun->shots, *prev = NULL; b != NULL; ) {
                        // Para tiros rotacionados como os do boss, b->x e b->y são o centro.
                        // Precisamos calcular o canto superior esquerdo da hitbox para a função `colisao`.
                        float adjusted_b_x = b->x - (boss_tiro_w / 2.0f);
                        float adjusted_b_y = b->y - (boss_tiro_h / 2.0f);

                        if (colisao(adjusted_b_x, adjusted_b_y, boss_tiro_w, boss_tiro_h,
                                    player_proj_hitbox_x, player_proj_hitbox_y,
                                    player.projectile_hitbox_width, player.projectile_hitbox_height)) {

                            player.vidas--;
                            player.timer_invencivel = 90; // Deixa o jogador invencível por 1.5 segundos (90 frames)

                            // Destrói o projétil
                            bullet *temp = b;
                            b = (bullet*)b->next;
                            if (prev == NULL) boss.gun->shots = (bullet*)temp->next;
                            else prev->next = temp->next;
                            bullet_destroy(temp);

                            break; // Se um tiro acertou, não precisamos checar os outros neste frame
                        } else {
                            prev = b;
                            b = (bullet*)b->next;
                        }
                    }
                }
                gerenciar_projeteis_fora_da_tela(player.gun, camera_x);
                gerenciar_projeteis_fora_da_tela(player.especial_gun, camera_x);

                for (int i = 0; i < MAX_VILOES; i++) gerenciar_projeteis_fora_da_tela(vetor_vilao[i].gun, camera_x);
                // Adicione a linha para gerenciar projéteis do boss fora da tela
                gerenciar_projeteis_fora_da_tela(boss.gun, camera_x);


                if (player.y > ZONA_MORTE) {
                    player.vidas--;
                    if (player.vidas > 0) {
                        player.x = player.respawn_x;
                        player.y = player.respawn_y;
                        player.vel_y = 0;
                        player.estado = PARADO;
                        player.pulos_restantes = MAX_PULOS;
                    }
                }

                if (player.vidas <= 0) {
                    if (texto_completo) free(texto_completo);
                    al_stop_timer(timer);
                    return ESTADO_GAME_OVER;
                }

                player.frame_contador++;
                if (player.frame_contador >= INTERVALO_ANIMACAO * FRAMES_ANDANDO) player.frame_contador = 0;
                player.frame_atual = player.frame_contador / INTERVALO_ANIMACAO;
            }

            redraw = true;
        }
        if (!vitoria && boss.estado == B_INATIVO && boss.hp <= 0) {
            vitoria = true; // Apenas define a flag de vitória
        }
        if (redraw && al_is_event_queue_empty(queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            camera_x = player.x - TELA_W / 2.0f;

            al_draw_bitmap(res->fundo_distante, -fmod(camera_x * 0.1, al_get_bitmap_width(res->fundo_distante)), 0, 0);
            al_draw_bitmap(res->fundo_distante, -fmod(camera_x * 0.1, al_get_bitmap_width(res->fundo_distante)) + al_get_bitmap_width(res->fundo_distante), 0, 0);
            al_draw_bitmap(res->fundo_meio, -fmod(camera_x * 0.3, al_get_bitmap_width(res->fundo_meio)), 0, 0);
            al_draw_bitmap(res->fundo_meio, -fmod(camera_x * 0.3, al_get_bitmap_width(res->fundo_meio)) + al_get_bitmap_width(res->fundo_meio), 0, 0);

            for (int i = 0; i < MAX_PLATAFORMAS; i++) al_draw_scaled_bitmap(plataformas[i].sprite, 0, 0, al_get_bitmap_width(plataformas[i].sprite), al_get_bitmap_height(plataformas[i].sprite), plataformas[i].x - camera_x, plataformas[i].y, plataformas[i].width, plataformas[i].height, 0);
            for (int i = 0; i < MAX_OBJETOS_MOVEIS; i++) al_draw_scaled_bitmap(objetos_moveis[i].sprite, 0, 0, al_get_bitmap_width(objetos_moveis[i].sprite), al_get_bitmap_height(objetos_moveis[i].sprite), objetos_moveis[i].x - camera_x, objetos_moveis[i].y, objetos_moveis[i].width, objetos_moveis[i].height, 0);
            for (int i = 0; i < MAX_VILOES; i++) desenhar_vilao(&vetor_vilao[i], camera_x, res);

            desenhar_boss(&boss, camera_x, res);

            for (int i = 0; i < MAX_ITENS; i++) {
                if (itens[i].ativo) {
                    ALLEGRO_BITMAP* sprite_atual_item = itens[i].sprite[itens[i].frame_atual]; // Renomeei a variável
                    al_draw_scaled_bitmap(sprite_atual_item, 0, 0,
                        al_get_bitmap_width(sprite_atual_item), al_get_bitmap_height(sprite_atual_item),
                        itens[i].x - camera_x, itens[i].y,
                        itens[i].width, itens[i].height, 0);
                }
            }

            ALLEGRO_BITMAP *sprite_atual_player = NULL; // Renomeei a variável
            if (player.esta_animando_poder) {
                int frame_index = player.frame_animacao_poder < FRAMES_PODER ? player.frame_animacao_poder : FRAMES_PODER - 1;
                sprite_atual_player = res->player_poder_anim[frame_index];
            }else {
                    switch(player.estado){
                        case ANDANDO: sprite_atual_player = res->player_andando[player.frame_atual]; break;
                        case PULANDO: sprite_atual_player = res->player_pulando[player.frame_atual]; break;
                        case ANDANDO_ATIRANDO: sprite_atual_player = res->player_andando_atirando[player.frame_atual]; break;
                        case PARADO_ATIRANDO: sprite_atual_player = res->player_atirando_parado; break;
                        case PARADO_ATIRANDO_CIMA: sprite_atual_player = res->player_atirando_cima; break;
                        case ABAIXANDO: sprite_atual_player = res->player_abaixando; break;
                        default: sprite_atual_player = res->player_parado; break;
                    }
            }
              // Lógica de piscar: só desenha o jogador se o timer de invencibilidade
            // for zero, OU se o frame atual for par (em uma contagem mais lenta).
            if (player.timer_invencivel <= 0 || (frame_geral / 4) % 2 == 0) {
                if(sprite_atual_player) {
                    al_draw_scaled_bitmap(sprite_atual_player, 0, 0, al_get_bitmap_width(sprite_atual_player), al_get_bitmap_height(sprite_atual_player),
                    TELA_W/2.0f - player.char_width/2.0f, player.y, player.char_width, player.char_height, player.virado_esquerda ? ALLEGRO_FLIP_HORIZONTAL : 0);
                }
            }

            if (player.vidas > 0) {
                float w = al_get_bitmap_width(res->sprites_vida[player.vidas - 1]);
                float h = al_get_bitmap_height(res->sprites_vida[player.vidas - 1]);
                al_draw_scaled_bitmap(res->sprites_vida[player.vidas - 1], 0, 0, w, h, 10, 10, w * 0.5, h * 0.5, 0);
            }

            // Desenho dos tiros do jogador (NORMAL)
            for (bullet *b = player.gun->shots; b != NULL; b = (bullet*)b->next) {
                float w_tiro = al_get_bitmap_width(res->tiro_player);
                float h_tiro = al_get_bitmap_height(res->tiro_player);
                float scale_tiro = 0.2f;

                if (b->vel_y < 0) { // Tiro para cima
                    al_draw_scaled_rotated_bitmap(res->tiro_player, w_tiro / 2.0f, h_tiro / 2.0f, b->x - camera_x, b->y, scale_tiro, scale_tiro, -ALLEGRO_PI / 2.0f, 0);
                } else if (b->vel_x < 0) { // Tiro para a esquerda
                    al_draw_scaled_bitmap(res->tiro_player, 0, 0, w_tiro, h_tiro, b->x - camera_x, b->y, w_tiro * scale_tiro, h_tiro * scale_tiro, ALLEGRO_FLIP_HORIZONTAL);
                } else { // Tiro para a direita (e outros casos)
                    al_draw_scaled_bitmap(res->tiro_player, 0, 0, w_tiro, h_tiro, b->x - camera_x, b->y, w_tiro * scale_tiro, h_tiro * scale_tiro, 0);
                }
            }

            // Desenho dos tiros ESPECIAIS do jogador
            for (bullet *b = player.especial_gun->shots; b != NULL; b = (bullet*)b->next) {
                float w_tiro_e = al_get_bitmap_width(res->tiro_especial);
                float h_tiro_e = al_get_bitmap_height(res->tiro_especial);
                float scale_tiro_e = 2.0f;

                 if (b->vel_y < 0) { // Tiro para cima
                    al_draw_scaled_rotated_bitmap(res->tiro_especial, w_tiro_e / 2.0f, h_tiro_e / 2.0f, b->x - camera_x, b->y, scale_tiro_e, scale_tiro_e, -ALLEGRO_PI / 2.0f, 0);
                } else if (b->vel_x < 0) { // Tiro para a esquerda
                    al_draw_scaled_bitmap(res->tiro_especial, 0, 0, w_tiro_e, h_tiro_e, b->x - camera_x, b->y, w_tiro_e * scale_tiro_e, h_tiro_e * scale_tiro_e, ALLEGRO_FLIP_HORIZONTAL);
                } else { // Tiro para a direita
                    al_draw_scaled_bitmap(res->tiro_especial, 0, 0, w_tiro_e, h_tiro_e, b->x - camera_x, b->y, w_tiro_e * scale_tiro_e, h_tiro_e * scale_tiro_e, 0);
                }
            }

            // Desenho dos tiros dos VILÕES
            for (int i = 0; i < MAX_VILOES; i++) {
                for (bullet *b = vetor_vilao[i].gun->shots; b != NULL; b = (bullet*)b->next) {
                    float w_tiro_v = al_get_bitmap_width(res->tiro_vilao);
                    float h_tiro_v = al_get_bitmap_height(res->tiro_vilao);
                    float scale_tiro_v = 2.0f;

                    if (b->vel_x < 0) { // Tiro para a esquerda
                        al_draw_scaled_bitmap(res->tiro_vilao, 0, 0, w_tiro_v, h_tiro_v, b->x - camera_x, b->y, w_tiro_v * scale_tiro_v, h_tiro_v * scale_tiro_v, ALLEGRO_FLIP_HORIZONTAL);
                    } else { // Tiro para a direita
                        al_draw_scaled_bitmap(res->tiro_vilao, 0, 0, w_tiro_v, h_tiro_v, b->x - camera_x, b->y, w_tiro_v * scale_tiro_v, h_tiro_v * scale_tiro_v, 0);
                    }
                }
            }

            // Desenho dos tiros do BOSS
            // Usa atan2 para rotacionar o sprite na direção exata da velocidade
            for (bullet *b = boss.gun->shots; b != NULL; b = (bullet*)b->next) {
                float w_tiro_boss = al_get_bitmap_width(res->tiro_vilao); // Reutilizando o sprite do tiro do vilão
                float h_tiro_boss = al_get_bitmap_height(res->tiro_vilao);
                float scale_tiro_boss = 2.5f; // Um pouco maior para o boss
                float angle = atan2(b->vel_y, b->vel_x); // Calcula o ângulo do movimento

                al_draw_scaled_rotated_bitmap(res->tiro_vilao, w_tiro_boss / 2.0f, h_tiro_boss / 2.0f, b->x - camera_x, b->y, scale_tiro_boss, scale_tiro_boss, angle, 0);
            }

            if (dialogo_ativo) {
                float caixa_x = 10, caixa_w = TELA_W - 20, caixa_h = 120, caixa_y = TELA_H - caixa_h - 10, padding = 10;
                al_draw_filled_rectangle(caixa_x, caixa_y, caixa_x + caixa_w, caixa_y + caixa_h, al_map_rgba(0, 0, 20, 180));
                al_draw_rectangle(caixa_x, caixa_y, caixa_x + caixa_w, caixa_y + caixa_h, al_map_rgb(100, 100, 255), 2.0);
                al_draw_multiline_text(res->fonte_dialo, al_map_rgb(255, 255, 255), caixa_x + padding, caixa_y +
                padding, caixa_w - (padding * 2), al_get_font_line_height(res->fonte_dialo) * 0.9, 0, texto_pagina_atual);
            }
            if (player.tem_poder_especial && cooldown_super <= 0) {
                if ((frame_geral / 30) % 2 == 0) {
                    al_draw_text(res->fonte_dialo,
                        al_map_rgb(255, 223, 0),
                        TELA_W - 15, 15,
                        ALLEGRO_ALIGN_RIGHT,
                        "SUPER DISPONÍVEL!"
                    );
                }
            }
             if (jogo_pausado && !dialogo_ativo && !player.esta_animando_poder) {
                al_draw_filled_rectangle(0, 0, TELA_W, TELA_H, al_map_rgba(0, 0, 0, 150));
                al_draw_text(res->fonte_dialo,
                                     al_map_rgb(255, 255, 255),
                                     TELA_W / 2,
                                     TELA_H / 2,
                                     ALLEGRO_ALIGN_CENTER,
                                     "JOGO PAUSADO");
            }
            if (boss.ativo) {
                desenhar_hud(res->fonte_dialo, &boss);
            }
            if (vitoria) {
                // Desenha uma camada escura semi-transparente para destacar o texto
                al_draw_filled_rectangle(0, 0, TELA_W, TELA_H, al_map_rgba(0, 0, 0, 150));

                // Desenha o texto de vitória
                al_draw_text(res->fonte_dialo, al_map_rgb(255, 215, 0), // Cor dourada
                                 TELA_W / 2, TELA_H / 2 - 40, ALLEGRO_ALIGN_CENTER, "VOCÊ VENCEU!");

                al_draw_text(res->fonte_dialo, al_map_rgb(255, 255, 255),
                                 TELA_W / 2, TELA_H / 2 + 20, ALLEGRO_ALIGN_CENTER, "Pressione ENTER para voltar ao Menu Principal");
            }

            al_flip_display();
        }
    }
}

// --- Implementação das Funções Auxiliares ---
void die(const char *mensagem) {
    fprintf(stderr, "Erro fatal: %s\n", mensagem);
    exit(1);
}

void inicializar_allegro() {
    if (!al_init()) die("Falha ao inicializar Allegro.");
    if (!al_install_keyboard()) die("Falha ao instalar teclado.");
    if (!al_install_mouse()) die("Falha ao inicializar mouse.");
    if (!al_init_image_addon()) die("Falha ao inicializar image addon.");
    if (!al_init_font_addon()) die("Falha ao inicializar font addon.");
    if (!al_init_ttf_addon()) die("Falha ao inicializar ttf addon.");
    if (!al_init_primitives_addon()) die("Falha ao inicializar primitives addon.");
}

void desenhar_menu(const GameResources *res, int opcao_selecionada) {
    float btn_w = 200.0f, btn_h = 80.0f;
    float btn_iniciar_x = TELA_W / 2 - btn_w / 2, btn_iniciar_y = TELA_H / 2 + 20;
    float btn_sair_x = TELA_W / 2 - btn_w / 2, btn_sair_y = TELA_H / 2 + 120;
    al_draw_scaled_bitmap(res->botao_iniciar, 0, 0, al_get_bitmap_width(res->botao_iniciar), al_get_bitmap_height(res->botao_iniciar), btn_iniciar_x, btn_iniciar_y, btn_w, btn_h, 0);
    al_draw_scaled_bitmap(res->botao_sair, 0, 0, al_get_bitmap_width(res->botao_sair), al_get_bitmap_height(res->botao_sair), btn_sair_x, btn_sair_y, btn_w, btn_h, 0);
    if (opcao_selecionada == 0) al_draw_rectangle(btn_iniciar_x - 5, btn_iniciar_y - 5, btn_iniciar_x + btn_w + 5, btn_iniciar_y + btn_h + 5, al_map_rgb(255, 255, 0), 3.0);
    else al_draw_rectangle(btn_sair_x - 5, btn_sair_y - 5, btn_sair_x + btn_w + 5, btn_sair_y + btn_h + 5, al_map_rgb(255, 255, 0), 3.0);
}

void load_resources(GameResources *res) {
    res->fundo_menu = al_load_bitmap("./fundo/fundo_menu.png");
    res->botao_iniciar = al_load_bitmap("./fundo/jogar.png");
    res->botao_sair = al_load_bitmap("./fundo/sair.png");
    res->botao_reiniciar = al_load_bitmap("./fundo/reiniciar.png");
    res->tela_go = al_load_bitmap("./fundo/gamer_over.png");
    res->fundo_distante = al_load_bitmap("./fundo/fundo.png");
    res->fundo_meio = al_load_bitmap("./fundo/fundo2.png");
    res->fonte_dialo = al_load_ttf_font("./fundo/letra.ttf",20,0);
    if(!res->fonte_dialo) die("Falha ao carregar fonte");
    res->pedra_sprites[0]  = al_load_bitmap("./fundo/pedra1.png");
    res->pedra_sprites[1]  = al_load_bitmap("./fundo/pedra3.png");
    res->pedra_sprites[2]  = al_load_bitmap("./fundo/pedra2.png");
    for (int i = 0; i < 3; i++) {
        if (!res->pedra_sprites[i]) die("Falha ao carregar um dos sprites de pedra.");
    }
    res->obj_movel_sprites[0] = al_load_bitmap("./fundo/nave.png");
    res->obj_movel_sprites[1] = al_load_bitmap("./fundo/pedra2.png");
    carregar_sprites_animacao(res->sprites_vida, MAX_VIDAS, "./personagens/megamente/vida/vida%d.png");
    carregar_sprites_animacao(res->sprites_item_vida, FRAMES_ITEM_VIDA, "./personagens/itens/vida_%d.png");
    carregar_sprites_animacao(res->sprites_item_poder, FRAMES_ITEM_VIDA, "./personagens/itens/poder_%d.png");
    res->player_parado = al_load_bitmap("./personagens/megamente/andando_ati/andando_0.png");
    res->player_atirando_parado = al_load_bitmap("./personagens/megamente/andando_ati/andando_0.png");
    res->player_abaixando = al_load_bitmap("./personagens/megamente/abaixando/abaixada.png");
    carregar_sprites_animacao(res->player_andando, FRAMES_ANDANDO, "personagens/megamente/andando_ati/andando_%d.png");
    carregar_sprites_animacao(res->player_pulando, FRAMES_PULANDO, "personagens/megamente/pulando_ati/pulando_ati_%d.png");
    carregar_sprites_animacao(res->player_andando_atirando, FRAMES_ATIRANDO, "personagens/megamente/andando_ati/andando_%d.png");
    carregar_sprites_animacao(res->vilao_andando, 6, "./personagens/vilao/andando/andando_%d.png");
    carregar_sprites_animacao(res->vilao_atirando, 3, "./personagens/vilao/atirando/atirando_%d.png");
    carregar_sprites_animacao(res->vilao_morrendo, 4, "./personagens/vilao/morrendo/morrendo_%d.png");
    carregar_sprites_animacao(res->player_poder_anim, FRAMES_PODER, "personagens/megamente/poder/poder_%d.png");

    res->tiro_player = al_load_bitmap("./personagens/megamente/bola_p/bola.png");
    res->tiro_especial = al_load_bitmap("./personagens/megamente/bola_p/super.png");
    res->tiro_vilao = al_load_bitmap("./personagens/vilao/atirando/tiro.png");
    res->player_atirando_cima = al_load_bitmap("./personagens/megamente/andando_ati/atirando.png");
    carregar_sprites_animacao(res->boss_andando, FRAMES_BOSS_ANDANDO, "./personagens/magneto/andando/andando_%d.png");
    carregar_sprites_animacao(res->boss_atacando, FRAMES_BOSS_ATACANDO, "./personagens/magneto/atacando/atirando_%d.png");
    carregar_sprites_animacao(res->boss_tomando_dano, FRAMES_BOSS_DANO, "./personagens/magneto/dano/dano_%d.png");
    res->boss_subindo_sprite = al_load_bitmap("./personagens/magneto/levitando/levitando.png");
    res->boss_voando_sprite = al_load_bitmap("./personagens/magneto/subindo/subindo.png");
    carregar_sprites_animacao(res->boss_morrendo, FRAMES_BOSS_MORRENDO, "./personagens/magneto/morrendo/morrendo_%d.png");

}

void destroy_resources(GameResources *res) {
    al_destroy_bitmap(res->fundo_menu);
    al_destroy_bitmap(res->botao_iniciar);
    al_destroy_bitmap(res->botao_sair);
    al_destroy_bitmap(res->tela_go);
    al_destroy_bitmap(res->fundo_distante);
    al_destroy_bitmap(res->fundo_meio);
    destruir_sprites_animacao(res->pedra_sprites, 3);
    destruir_sprites_animacao(res->obj_movel_sprites, 2);
    destruir_sprites_animacao(res->sprites_vida, MAX_VIDAS);
    destruir_sprites_animacao(res->sprites_item_vida, FRAMES_ITEM_VIDA);
    al_destroy_bitmap(res->player_parado);
    al_destroy_bitmap(res->player_atirando_parado);
    al_destroy_bitmap(res->player_abaixando);
    al_destroy_font(res->fonte_dialo);
    al_destroy_bitmap(res->player_atirando_cima);
    al_destroy_bitmap(res->tiro_especial);
    al_destroy_bitmap(res->botao_reiniciar);
    destruir_sprites_animacao(res->boss_andando, FRAMES_BOSS_ANDANDO);
    destruir_sprites_animacao(res->boss_atacando, FRAMES_BOSS_ATACANDO);
    destruir_sprites_animacao(res->boss_tomando_dano, FRAMES_BOSS_DANO);
    if(res->boss_subindo_sprite) al_destroy_bitmap(res->boss_subindo_sprite);
    if(res->boss_voando_sprite) al_destroy_bitmap(res->boss_voando_sprite);
    destruir_sprites_animacao(res->boss_morrendo, FRAMES_BOSS_MORRENDO);
    destruir_sprites_animacao(res->player_andando, FRAMES_ANDANDO);
    destruir_sprites_animacao(res->player_pulando, FRAMES_PULANDO);
    destruir_sprites_animacao(res->player_andando_atirando, FRAMES_ATIRANDO);
    destruir_sprites_animacao(res->vilao_andando, 6);
    destruir_sprites_animacao(res->vilao_atirando, 3);
    destruir_sprites_animacao(res->vilao_morrendo, 4);
    destruir_sprites_animacao(res->player_poder_anim, FRAMES_PODER);
    al_destroy_bitmap(res->tiro_player);
    al_destroy_bitmap(res->tiro_vilao);
}

void carregar_sprites_animacao(ALLEGRO_BITMAP *sprites[], int num_frames, const char *formato_path) {
    for (int i = 0; i < num_frames; i++) {
        char path[128];
        snprintf(path, sizeof(path), formato_path, i);
        sprites[i] = al_load_bitmap(path);
        if (!sprites[i]) {
            char msg_erro[160];
            snprintf(msg_erro, sizeof(msg_erro), "Falha ao carregar sprite: %s", path);
            die(msg_erro);
        }
    }
}

void destruir_sprites_animacao(ALLEGRO_BITMAP *sprites[], int num_frames) {
    for (int i = 0; i < num_frames; i++) {
        if(sprites[i]) al_destroy_bitmap(sprites[i]);
    }
}

bool colisao(float r1_x, float r1_y, float r1_w, float r1_h, float r2_x, float r2_y, float r2_w, float r2_h) {
    return (r1_x < r2_x + r2_w) && (r1_x + r1_w > r2_x) && (r1_y < r2_y + r2_h) && (r1_y + r1_h > r2_y);
}

void reiniciar_jogo(Personagem *player, Plataforma plataformas[], GameResources *res) {
    player->char_width = al_get_bitmap_width(res->player_parado) * ESCALA_JOGADOR;
    player->char_height = al_get_bitmap_height(res->player_parado) * ESCALA_JOGADOR;

    // Hitbox geral (para colisão com plataformas, contato com inimigos)
    player->hitbox_width = player->char_width * 0.6f;
    player->hitbox_height = player->char_height * 0.85f;
    player->hitbox_x_offset = (player->char_width - player->hitbox_width) / 2.0f;
    player->hitbox_y_offset = (player->char_height - player->hitbox_height);

    // Hitbox para projéteis (menor e mais centralizada)
    player->projectile_hitbox_width = player->char_width * 0.3f; // Ex: 30% da largura do sprite
    player->projectile_hitbox_height = player->char_height * 0.4f; // Ex: 40% da altura do sprite
    player->projectile_hitbox_x_offset = (player->char_width - player->projectile_hitbox_width) / 2.0f;
    player->projectile_hitbox_y_offset = (player->char_height - player->projectile_hitbox_height) / 2.0f; // Centraliza verticalmente

    player->x = plataformas[0].x + (plataformas[0].width / 2) - (player->char_width / 2);
    player->y = (plataformas[0].y + AJUSTE_VERTICAL_PEDRA) - player->char_height;
    player->respawn_x = player->x;
    player->respawn_y = player->y;
    player->vidas = MAX_VIDAS;
    player->vel_x = 0;
    player->vel_y = 0;
    player->estado = PARADO;
    player->frame_atual = 0;
    player->frame_contador = 0;
    player->timer_invencivel = 0;
    player->pulos_restantes = MAX_PULOS;
    player->virado_esquerda = false;
    player->tem_poder_especial = false;
    player->esta_animando_poder = false;

    if (player->gun) pistol_destroy(player->gun);
    player->gun = pistol_create();
    if (player->especial_gun) pistol_destroy(player->especial_gun);
    player->especial_gun = pistol_create();
}

void reiniciar_vilao(Vilao *v, Plataforma plataformas[], int indice_vilao, GameResources *res) {
    v->width = al_get_bitmap_width(res->vilao_andando[0]) * 1.5f;
    v->height = al_get_bitmap_height(res->vilao_andando[0]) * 1.5f;
    v->vidas = VIDA_VILAO;
    v->estado = V_ANDANDO;
    v->ativo = true;
    v->vel_x = 2.0f;
    v->virado_esquerda = (indice_vilao % 2 != 0);
    v->frame_atual = 0;
    v->frame_contador = 0;
    if (v->gun) pistol_destroy(v->gun);
    v->gun = pistol_create();

    switch (indice_vilao) {
        case 0: v->id_plataforma_patrulha = 8; break;
        case 1: v->id_plataforma_patrulha = 12; break;
        case 2: v->id_plataforma_patrulha = 13; break;
        case 3: v->id_plataforma_patrulha = 18; break;
        case 4: v->id_plataforma_patrulha = 20; break;
        case 5: v->id_plataforma_patrulha = 22; break;
    }
    Plataforma p_patrulha = plataformas[v->id_plataforma_patrulha];
    v->x = p_patrulha.x + (p_patrulha.width / 2) - (v->width / 2);
    v->y = (p_patrulha.y + AJUSTE_VERTICAL_PEDRA) - v->height;
    v->limite_esq = p_patrulha.x;
    v->limite_dir = p_patrulha.x + p_patrulha.width - v->width;
}
void inicializar_boss(Boss *b, Plataforma *plat, GameResources *res) {
    *b = (Boss){0};
    b->width = al_get_bitmap_width(res->boss_andando[0]) * ESCALA_BOSS;
    b->height = al_get_bitmap_height(res->boss_andando[0]) * ESCALA_BOSS;
    b->ativo = false;
    b->estado = B_ANDANDO;
    b->virado_esquerda = true;
    b->gun = pistol_create();
    b->max_hp = VIDA_MAX_BOSS;
    b->hp = b->max_hp;
    b->limite_esq = plat->x;
    b->limite_dir = plat->x + plat->width - b->width;
    b->x = b->limite_dir - 50;
    b->y = plat->y - b->height;
    b->tiros_na_rajada = 0;
    b->timer_pausa_ataque = 0;
    b->timer_avanco = 0;
}
// Esta é a versão completa e atualizada da função.
// Copie e cole este bloco inteiro sobre a sua função antiga.
void atualizar_boss_batalha(Boss *b, Personagem *p, Plataforma *plat_chao, int id_plataforma_player, ALLEGRO_KEYBOARD_STATE *ks) {
    // 1. Garante que os projéteis do boss sempre se movam, independentemente do estado dele.
    bullet_move(b->gun->shots);
    // 2. Lógica de Ativação: O Boss só "acorda" se estiver inativo e o jogador pisar na plataforma.
    if (!b->ativo) {
        if (id_plataforma_player == PLATAFORMA_BOSS_IDX) {
            b->ativo = true;
            b->estado = B_ANDANDO; // Ele começa a batalha no chão.
        } else {
            return; // Se não for ativado, a função para por aqui.
        }
    }

    // 3. Timer de invulnerabilidade após levar dano.
    if (b->timer_dano > 0) {
        b->timer_dano--;
    }

    // 4. Gatilho da Fase Aérea: Se a vida está na metade e ele está no chão, ele começa a subir.
    if (b->hp <= b->max_hp / 2 && (b->estado == B_ANDANDO || b->estado == B_ATIRANDO_CHAO)) {
        b->estado = B_SUBINDO_PARA_VOAR;
    }

    // 5. Máquina de Estados principal que controla o comportamento do Boss.
    switch (b->estado) {
        // --- FASES TERRESTRES ---
        case B_ANDANDO:
            // Lógica de patrulha
            b->vel_x = b->virado_esquerda ? -VELOCIDADE_VILAO : VELOCIDADE_VILAO;
            b->x += b->vel_x;
            if ((b->virado_esquerda && b->x <= b->limite_esq) || (!b->virado_esquerda && b->x >= b->limite_dir)) {
                b->virado_esquerda = !b->virado_esquerda;
            }
            // Se o jogador estiver perto, inicia o ciclo de ataque preparando a rajada
            if (fabs((p->x + p->char_width / 2) - (b->x + b->width / 2)) < 500) { // Aumentei um pouco a distância de detecção
                b->estado = B_ATIRANDO_CHAO;
                b->frame_atual = 0;
                b->tiros_na_rajada = 3; // Prepara para uma rajada de 3 tiros
            }
            break;

        case B_ATIRANDO_CHAO:
            b->vel_x = 0; // Para de andar para atirar
            b->virado_esquerda = (p->x < b->x); // Vira para o jogador

            if (b->gun->timer > 0) b->gun->timer--;

            // Se o timer zerou E ainda há tiros na rajada
            if (b->gun->timer == 0 && b->tiros_na_rajada > 0) {
                pistol_shot_horizontal(b->gun, b->x + b->width / 2, b->y + b->height / 2, b->virado_esquerda);
                b->gun->timer = 30; // Cooldown de 0.5s entre os tiros da rajada
                b->tiros_na_rajada--;
            }

            // Se a rajada terminou, inicia o avanço
            if (b->tiros_na_rajada <= 0) {
                b->estado = B_AVANCANDO;
                b->timer_avanco = 60; // Avança por 1 segundo (60 frames)
            }
            break;
        case B_AVANCANDO:
            // Vira para o jogador e avança
            b->virado_esquerda = (p->x < b->x);
            b->vel_x = b->virado_esquerda ? -VELOCIDADE_VILAO * 1.5f : VELOCIDADE_VILAO * 1.5f; // Avança um pouco mais rápido
            b->x += b->vel_x;

            // Garante que o boss não saia da plataforma
            if (b->x < b->limite_esq) b->x = b->limite_esq;
            if (b->x > b->limite_dir) b->x = b->limite_dir;

            // Decrementa o timer de avanço
            b->timer_avanco--;
            if (b->timer_avanco <= 0) {
                b->estado = B_ANDANDO; // Retorna para a patrulha, completando o ciclo
            }
            break;

        // --- FASES AÉREAS E TRANSIÇÃO ---
        case B_SUBINDO_PARA_VOAR:
            b->y -= VELOCIDADE_SUBIDA;
            if (b->y <= ALTURA_VOO) {
                b->y = ALTURA_VOO;
                b->estado = B_VOANDO_ATACANDO;
                b->vel_x = VELOCIDADE_VILAO_VOANDO;
                b->tiros_na_rajada = 5; // Prepara a primeira rajada de 5 tiros.
            }
            break;

        case B_VOANDO_ATACANDO:
            // Movimento de patrulha no ar, limitado pela plataforma.
            b->x += b->vel_x;
            if (b->x <= plat_chao->x || b->x + b->width >= plat_chao->x + plat_chao->width) {
                b->vel_x *= -1;
            }
            b->virado_esquerda = (p->x < b->x); // Sempre vira para o jogador

            // Lógica de tiro em rajada
            if (b->gun->timer > 0) b->gun->timer--;
            if (b->gun->timer == 0 && b->tiros_na_rajada > 0) {
                pistol_shot_direcionado(b->gun, b->x + b->width / 2, b->y + b->height, p->x + p->char_width / 2, p->y + p->char_height / 2);
                b->gun->timer = 20; // Cooldown curto entre os tiros da rajada
                b->tiros_na_rajada--;
            }

            // Se a rajada terminou, inicia a pausa.
            if (b->tiros_na_rajada <= 0) {
                b->estado = B_VOANDO_PAUSADO;
                b->timer_pausa_ataque = 120; // Pausa por 2 segundos (a 60 FPS).
            }
            break;

        case B_VOANDO_PAUSADO:
            // Continua voando, mas sem atirar.
            b->x += b->vel_x;
            if (b->x <= plat_chao->x || b->x + b->width >= plat_chao->x + plat_chao->width) {
                b->vel_x *= -1;
            }
            b->virado_esquerda = (p->x < b->x);

            // Decrementa o timer da pausa.
            if (b->timer_pausa_ataque > 0) b->timer_pausa_ataque--;

            // Quando a pausa acaba, prepara a próxima rajada.
            if (b->timer_pausa_ataque <= 0) {
                b->estado = B_VOANDO_ATACANDO;
                b->tiros_na_rajada = 5; // Reseta o contador de tiros.
            }
            break;

        // --- ESTADOS DE DANO E MORTE ---
        case B_TOMANDO_DANO:
            b->vel_x = 0;
            // A animação e a volta ao estado anterior são controladas pela lógica de frames abaixo.
            break;

        case B_MORRENDO_IMPACTO:
            b->vel_x = 0;
            break;

        case B_MORRENDO_CAINDO:
            b->y += GRAVIDADE_MORTE;
            if (b->y + b->height >= plat_chao->y) {
                b->y = plat_chao->y - b->height;
                b->estado = B_MORRENDO_NO_CHAO;
                b->frame_atual = 6; // Pula para os frames da animação no chão
            }
            break;

        case B_MORRENDO_NO_CHAO:
        case B_INATIVO:
            b->vel_x = 0;
            break;
    }

    // 6. Lógica de Animação (controla a troca de frames para cada estado)
    b->frame_contador++;
    if (b->frame_contador >= INTERVALO_ANIMACAO) {
        b->frame_contador = 0;
        int max_frames = 0;
           // ESTE É O SWITCH COM OS AVISOS
        switch(b->estado) {
            case B_ANDANDO:             max_frames = FRAMES_BOSS_ANDANDO; break;
            case B_AVANCANDO:
            case B_ATIRANDO_CHAO:       max_frames = FRAMES_BOSS_ATACANDO; break;
            case B_TOMANDO_DANO:        max_frames = FRAMES_BOSS_DANO; break;
            case B_MORRENDO_IMPACTO:
            case B_MORRENDO_CAINDO:
            case B_MORRENDO_NO_CHAO:

                max_frames = FRAMES_BOSS_MORRENDO; break;

            // Estes estados usam sprites únicos, então não precisam de um max_frames.
            // Adicioná-los aqui apenas informa ao compilador que não os esquecemos.
            case B_SUBINDO_PARA_VOAR:
            case B_VOANDO_ATACANDO:
            case B_VOANDO_PAUSADO:
            case B_INATIVO:
                break;
        }

        if (max_frames > 0) {
            b->frame_atual++;
            if (b->estado == B_MORRENDO_IMPACTO && b->frame_atual >= 2) {
                b->estado = B_MORRENDO_CAINDO;
                b->frame_atual = 2;
            } else if (b->estado == B_MORRENDO_CAINDO && b->frame_atual >= 6) {
                b->frame_atual = 2; // Loop nos frames de queda
            } else if (b->estado == B_MORRENDO_NO_CHAO && b->frame_atual >= max_frames) {
                b->frame_atual = max_frames - 1; // Trava no último frame
                b->ativo = false; // Desativa o boss permanentemente
                b->estado = B_INATIVO;
            } else if (b->frame_atual >= max_frames) {
                if (b->estado == B_TOMANDO_DANO) {
                    // Retorna ao estado que estava antes de tomar dano
                    b->estado = (b->y < ALTURA_VOO + 50) ? B_VOANDO_PAUSADO : B_ANDANDO;
                }
                b->frame_atual = 0;
            }
        }
    }
}



void atualizar_vilao(Vilao *v, Personagem *p, int id_plataforma_player) {
    bullet_move(v->gun->shots);
    if (!v->ativo) return;
    switch (v->estado) {
        case V_MORRENDO:
            v->frame_contador++;
            if (v->frame_contador >= INTERVALO_ANIMACAO * 2) {
                v->frame_contador = 0;
                v->frame_atual++;
                if (v->frame_atual >= 4) { v->estado = V_INATIVO; v->ativo = false; }
            }
            break;
        case V_ANDANDO:
        case V_ATIRANDO:
            v->estado = (id_plataforma_player == v->id_plataforma_patrulha) ? V_ATIRANDO : V_ANDANDO;
            if (v->estado == V_ATIRANDO) {
                v->vel_x = 0;
                v->virado_esquerda = (p->x < v->x);
                if (v->gun->timer > 0) v->gun->timer--;

            if (v->gun->timer == 0) {
                // Posição inicial do tiro do vilão
                unsigned short start_x = v->x + (v->virado_esquerda ? 0 : v->width);
                unsigned short start_y = v->y + (v->height * 0.3f);

                // Determina a direção correta usando o enum
                DirecaoTiro direcao_tiro_vilao = v->virado_esquerda ? DIR_ESQUERDA : DIR_DIREITA;

                // Chama pistol_shot com a direção correta
                v->gun->shots = pistol_shot(start_x, start_y, direcao_tiro_vilao, v->gun);

                v->gun->timer = PISTOL_COOLDOWN * 5;
            }
            } else {
                v->vel_x = v->virado_esquerda ? -2.0f : 2.0f;
                v->x += v->vel_x;
                if (v->x <= v->limite_esq) { v->x = v->limite_esq; v->virado_esquerda = false; }
                else if (v->x >= v->limite_dir) { v->x = v->limite_dir; v->virado_esquerda = true; }
            }
            v->frame_contador++;
            if (v->frame_contador >= INTERVALO_ANIMACAO) {
                v->frame_contador = 0;
                int max_frames = (v->estado == V_ANDANDO) ? 6 : 3;
                v->frame_atual = (v->frame_atual + 1) % max_frames;
            }
            break;
        case V_INATIVO: break;
    }
}

void desenhar_vilao(const Vilao *v, float camera_x, const GameResources *res) {
    if (!v->ativo) return;
    ALLEGRO_BITMAP *sprite_atual = NULL;
    switch (v->estado) {
        case V_ANDANDO:  sprite_atual = res->vilao_andando[v->frame_atual]; break;
        case V_ATIRANDO: sprite_atual = res->vilao_atirando[v->frame_atual]; break;
        case V_MORRENDO: sprite_atual = res->vilao_morrendo[v->frame_atual]; break;
        default: return;
    }
    if (sprite_atual) al_draw_scaled_bitmap(sprite_atual, 0, 0, al_get_bitmap_width(sprite_atual), al_get_bitmap_height(sprite_atual), v->x - camera_x, v->y, v->width, v->height, v->virado_esquerda ? ALLEGRO_FLIP_HORIZONTAL : 0);
}

// Versão completa e corrigida da função de desenho do Boss.
void desenhar_boss(const Boss *b, float camera_x, const GameResources *res) {
    if (!b->ativo) return;

    ALLEGRO_BITMAP* sprite_atual = NULL;
    int flip = b->virado_esquerda ? ALLEGRO_FLIP_HORIZONTAL : 0;

    switch(b->estado) {
        // Corrigido de V_ANDANDO para B_ANDANDO
        case B_ANDANDO:
            sprite_atual = res->boss_andando[b->frame_atual];
            break;
        case B_AVANCANDO:
            sprite_atual = res->boss_andando[b->frame_atual];
        case B_ATIRANDO_CHAO:
            sprite_atual = res->boss_atacando[b->frame_atual];
            break;
        case B_TOMANDO_DANO:
            sprite_atual = res->boss_tomando_dano[b->frame_atual];
            break;
        case B_SUBINDO_PARA_VOAR:
            sprite_atual = res->boss_subindo_sprite;
            break;

        // Agrupamos os dois estados de voo para usar o mesmo sprite.
        case B_VOANDO_ATACANDO:
        case B_VOANDO_PAUSADO: // <-- Adicionado o caso que faltava.
            sprite_atual = res->boss_voando_sprite;
            break;

        case B_MORRENDO_IMPACTO:
        case B_MORRENDO_CAINDO:
        case B_MORRENDO_NO_CHAO:
            sprite_atual = res->boss_morrendo[b->frame_atual];
            break;

        case B_INATIVO:
            return;
    }

    if (sprite_atual) {
        al_draw_scaled_bitmap(sprite_atual, 0, 0, al_get_bitmap_width(sprite_atual), al_get_bitmap_height(sprite_atual),
            b->x - camera_x, b->y, b->width, b->height, flip);
    }
}
void desenhar_hud(ALLEGRO_FONT *fonte, const Boss *b) {
    // Largura da barra de HP
    float bar_width = 300.0f;
    // Altura da barra de HP
    float bar_height = 30.0f;
    // Padding da borda direita e superior
    float padding_x = 10.0f;
    float padding_y = 10.0f;

    // Calcula a posição X para alinhar à direita
    float start_x = TELA_W - bar_width - padding_x;
    float start_y = padding_y;

    // Fundo da barra de HP (vermelho escuro)
    al_draw_filled_rectangle(start_x, start_y, start_x + bar_width, start_y + bar_height, al_map_rgb(50,0,0));

    // Preenchimento da barra de HP (vermelho)
    float hp_percent = (float)b->hp / b->max_hp;
    if (hp_percent < 0) hp_percent = 0;
    al_draw_filled_rectangle(start_x, start_y, start_x + (bar_width * hp_percent), start_y + bar_height, al_map_rgb(200,0,0));

    // Borda da barra de HP (branca)
    al_draw_rectangle(start_x, start_y, start_x + bar_width, start_y + bar_height, al_map_rgb(255,255,255), 2);

    // Texto "BOSS HP"
    // Alinha o texto à esquerda da barra (dentro do padding)
    al_draw_text(fonte, al_map_rgb(255,255,255), start_x + 5, start_y + 5, 0, "BOSS HP");

   
}
void gerenciar_projeteis_fora_da_tela(pistol *gun, float camera_x) {
    if (!gun || !gun->shots) return;
    for (bullet *b = gun->shots, *prev = NULL; b != NULL; ) {
        // Aumenta a margem de segurança para fora da tela
        if (b->x > camera_x + TELA_W + 100 || b->x < camera_x - 100 || b->y < -100 || b->y > TELA_H + 100) {
            bullet *temp = b;
            b = (bullet*)b->next;
            if (prev == NULL) gun->shots = (bullet*)temp->next;
            else prev->next = temp->next;
            bullet_destroy(temp);
        } else {
            prev = b;
            b = (bullet*)b->next;
        }
    }
}
char* carregar_texto_de_arquivo(const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        fprintf(stderr, "AVISO: Nao foi possivel abrir o arquivo de dialogo: %s\n", nome_arquivo);
        return NULL;
    }

    // Calcula o tamanho do arquivo
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    rewind(arquivo);

    // Aloca memória para o conteúdo + 1 para o caractere nulo '\0'
    char* buffer = (char*)malloc(tamanho + 1);
    if (!buffer) {
        fprintf(stderr, "ERRO: Falha ao alocar memoria para o dialogo.\n");
        fclose(arquivo);
        return NULL;
    }

    // Lê o arquivo para o buffer
    long resultado_leitura = fread(buffer, 1, tamanho, arquivo);
    if (resultado_leitura != tamanho) {
        fprintf(stderr, "AVISO: Erro ao ler o arquivo de dialogo: %s\n", nome_arquivo);
        free(buffer);
        fclose(arquivo);
        return NULL;
    }

    // Adiciona o terminador nulo para formar uma string válida
    buffer[tamanho] = '\0';

    fclose(arquivo);
    return buffer;
}