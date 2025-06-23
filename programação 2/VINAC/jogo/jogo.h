#ifndef __JOGO__
#define __JOGO__
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
#include "Pistol.h"
#include "pistol_boss.h"
#include <stdbool.h>
#include "Bullet.h" // Garanta que está incluindo o arquivo correto

// --- Constantes Globais ---
#define TELA_W 900
#define TELA_H 800
#define FPS 60

// --- Constantes do Jogo ---
#define MAX_PLATAFORMAS 29
#define MAX_OBJETOS_MOVEIS 2
#define MAX_VIDAS 5
#define MAX_PULOS 2 
#define MAX_VILOES 6
#define MAX_ITENS 2

// --- Constantes do Jogador ---
#define GRAVIDADE 0.5f
#define FORCA_PULO -12.0f
#define VELOCIDADE_JOGADOR 5.0f
#define ESCALA_JOGADOR 3.0f
#define ESCALA_BOSS 2.0f

// --- Constantes de Animação ---
#define FRAMES_PODER 11     
#define FRAMES_ANDANDO 10
#define FRAMES_PULANDO 7
#define FRAMES_ATIRANDO 10
#define INTERVALO_ANIMACAO 8
#define FRAMES_ITEM_VIDA 9
#define INTERVALO_ANIMACAO_ITEM 10 
#define ESCALA_ITEM_VIDA 0.35f
#define PISTOL_COOLDOWN 30
#define VIDA_VILAO 2;
#define COOLDOWN_SUPER_SEGUNDOS 10.0f


#define PLATAFORMA_ITEM_VIDA 26
#define PLATAFORMA_ITEM_PODER 1
#define ZONA_MORTE (TELA_H + 50)
#define ALTURA_FLUTUACAO_ITEM 50.0f 
#define AJUSTE_VERTICAL_PEDRA 25.0f           
#define INTERVALO_ANIMACAO_PODER 12 
#define ALTURA_FLUTUACAO_PODER 80.0f   

#define FRAMES_BOSS_ANDANDO 8
#define FRAMES_BOSS_ATACANDO 6
#define FRAMES_BOSS_DANO 3
#define FRAMES_BOSS_MORRENDO 21
#define INTERVALO_ANIMACAO 8
#define VELOCIDADE_VILAO 2.0f
#define VELOCIDADE_VILAO_VOANDO 3.0f
#define VIDA_MAX_BOSS 10
#define ALTURA_VOO 100.0f
#define VELOCIDADE_SUBIDA 2.0f
#define GRAVIDADE_MORTE 4.0f
#define PLATAFORMA_BOSS_IDX 29



// --- Estados da Máquina de Estados ---
typedef enum {ESTADO_MENU,ESTADO_JOGANDO,ESTADO_GAME_OVER,ESTADO_SAIR } GameState;
typedef enum{B_VOANDO}BossState;
// --- Estados do Personagem e Vilão ---
typedef enum { PARADO, ANDANDO, PULANDO, ABAIXANDO, ANDANDO_ATIRANDO, PARADO_ATIRANDO ,PARADO_ATIRANDO_CIMA} EstadoPersonagem;
typedef enum { V_ANDANDO, V_ATIRANDO, V_MORRENDO, V_INATIVO } EstadoVilao;
typedef enum {TIPO_ITEM_VIDA,TIPO_ITEM_PODER} ItemType;
typedef enum {
    B_ANDANDO, B_ATIRANDO_CHAO,B_AVANCANDO, B_TOMANDO_DANO, 
    B_SUBINDO_PARA_VOAR, B_VOANDO_ATACANDO,B_VOANDO_PAUSADO,
    B_MORRENDO_IMPACTO,
    B_MORRENDO_CAINDO,
    B_MORRENDO_NO_CHAO,
    B_INATIVO
} VilaoState;

// --- Estruturas de Dados ---
typedef struct {
    ALLEGRO_BITMAP *sprite;
    float x, y;
    float width, height;
} Plataforma;

typedef struct {
    ALLEGRO_BITMAP *sprite;
    float x, y;
    float width, height;
    float start_x, end_x;
    float move_speed;
    int move_direction; 
} ObjetoMovel;

typedef struct {
    float x, y, vel_x;
    float width, height;
    bool virado_esquerda, ativo;
    VilaoState estado;
    pistol *gun;
    int frame_atual, frame_contador;
    int hp, max_hp;
    float limite_esq, limite_dir;
    float timer_dano;
    int tiros_na_rajada;      // Contador para a rajada de 5 tiros
    float timer_pausa_ataque; // Timer para a pausa entre as rajadas
    float timer_avanco;
} Boss;

typedef struct {
    float x, y, width, height;
    bool ativo;
    int frame_atual, frame_contador;
    ItemType tipo;
    ALLEGRO_BITMAP **sprite;
    int num_frames;
    int intervalo_animacao;
} Item;

typedef struct {
    float x, y, vel_x, vel_y;
    float respawn_x, respawn_y;
    float char_width, char_height; 
    float hitbox_x_offset, hitbox_y_offset, hitbox_width, hitbox_height;
    int vidas;
    EstadoPersonagem estado;
    pistol *gun;
    pistol *especial_gun;
    int frame_atual, frame_contador;
    int pulos_restantes;
    bool virado_esquerda;
    bool tem_poder_especial;
    bool esta_animando_poder;     // true quando a animação especial está tocando
    int frame_animacao_poder;     // Frame atual da animação (de 0 a 4)
    int contador_frame_poder;
    float timer_invencivel; 
    float projectile_hitbox_width;
    float projectile_hitbox_height;
    float projectile_hitbox_x_offset;
    float projectile_hitbox_y_offset;
} Personagem;

typedef struct {
    float x, y, vel_x;
    float width, height;
    int vidas;
    EstadoVilao estado;
    pistol *gun;
    int frame_atual, frame_contador;
    bool virado_esquerda;
    bool ativo; 
    int id_plataforma_patrulha;
    float limite_esq, limite_dir;
} Vilao;

// --- Estrutura para Agrupar Recursos ---
typedef struct {
    // UI
    ALLEGRO_BITMAP *fundo_menu;
    ALLEGRO_BITMAP *botao_iniciar;
    ALLEGRO_BITMAP *botao_sair;
    ALLEGRO_BITMAP *botao_reiniciar;
    ALLEGRO_BITMAP *tela_go;

    // Cenário
    ALLEGRO_BITMAP *fundo_distante;
    ALLEGRO_BITMAP *fundo_meio;
    ALLEGRO_BITMAP *pedra_sprites[3];
    ALLEGRO_BITMAP *obj_movel_sprites[2];
    ALLEGRO_FONT   *fonte_dialo;

    // Personagem e Itens
    ALLEGRO_BITMAP *sprites_vida[MAX_VIDAS];
    ALLEGRO_BITMAP *sprites_item_vida[FRAMES_ITEM_VIDA];
    ALLEGRO_BITMAP *sprites_item_poder[FRAMES_ITEM_VIDA];
    ALLEGRO_BITMAP *player_parado;
    ALLEGRO_BITMAP *player_atirando_parado;
    ALLEGRO_BITMAP *player_abaixando;
    ALLEGRO_BITMAP *player_andando[FRAMES_ANDANDO];
    ALLEGRO_BITMAP *player_pulando[FRAMES_PULANDO];
    ALLEGRO_BITMAP *player_andando_atirando[FRAMES_ATIRANDO];
    ALLEGRO_BITMAP *player_poder_anim[FRAMES_PODER]; 
    ALLEGRO_BITMAP *player_atirando_cima;


    // Vilões e Projéteis
    ALLEGRO_BITMAP *vilao_andando[6];
    ALLEGRO_BITMAP *vilao_atirando[3];
    ALLEGRO_BITMAP *vilao_morrendo[4];
    ALLEGRO_BITMAP *tiro_player;
    ALLEGRO_BITMAP *tiro_vilao;
    ALLEGRO_BITMAP *tiro_especial;

    //Boss
    ALLEGRO_BITMAP *boss_andando[FRAMES_BOSS_ANDANDO];
    ALLEGRO_BITMAP *boss_atacando[FRAMES_BOSS_ATACANDO];
    ALLEGRO_BITMAP *boss_tomando_dano[FRAMES_BOSS_DANO];
    ALLEGRO_BITMAP *boss_subindo_sprite;
    ALLEGRO_BITMAP *boss_voando_sprite;
    ALLEGRO_BITMAP *boss_morrendo[FRAMES_BOSS_MORRENDO];
} GameResources;

void die(const char *mensagem);
void inicializar_allegro();
bool colisao(float r1_x, float r1_y, float r1_w, float r1_h, float r2_x, float r2_y, float r2_w, float r2_h);
void carregar_sprites_animacao(ALLEGRO_BITMAP *sprites[], int num_frames, const char *formato_path);
void destruir_sprites_animacao(ALLEGRO_BITMAP *sprites[], int num_frames);

void reiniciar_jogo(Personagem *player, Plataforma plataformas[], GameResources *res);
void reiniciar_vilao(Vilao *v, Plataforma plataformas[], int id, GameResources *res);
void atualizar_vilao(Vilao *v, Personagem *p, int id_plataforma_player);
void desenhar_vilao(const Vilao *v, float camera_x, const GameResources *res);
void gerenciar_projeteis_fora_da_tela(pistol *gun, float camera_x); 
void desenhar_menu(const GameResources *res, int opcao_selecionada);
bool esta_nCamera(float obj_x, float obj_width, float camera_x);
GameState executar_menu(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, GameResources *res);
GameState executar_jogo(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, GameResources *res);
GameState executar_game_over(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, GameResources *res);
char* carregar_texto_de_arquivo(const char* nome_arquivo);
void load_resources(GameResources *res);
void destroy_resources(GameResources *res);
bool proxima_pagina(char* pagina_buffer, int tamanho_buffer, char** ponteiro_leitura, int num_linhas);

void atualizar_boss(Boss *boss);
void desenhar_boss(const Boss *b, float camera_x, const GameResources *res);
void desenhar_hud(ALLEGRO_FONT *fonte, const Boss *b);
void inicializar_boss(Boss *b, Plataforma *plat, GameResources *res);
void atualizar_boss_batalha(Boss *b, Personagem *p, Plataforma *plat_chao, int id_plataforma_player, ALLEGRO_KEYBOARD_STATE *ks);


#endif