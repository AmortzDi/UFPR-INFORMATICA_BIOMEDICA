// Conteúdo do arquivo Pistol.c
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>   

#include "Pistol.h"

#define PLAYER_BULLET_SPEED 10.0f // Velocidade para os tiros do jogador

pistol* pistol_create() {
    pistol *new_pistol = (pistol*) malloc(sizeof(pistol));
    if (!new_pistol) return NULL;
    new_pistol->timer = 0;
    new_pistol->shots = NULL;
    return new_pistol;
}

// ATENÇÃO: Função pistol_shot MODIFICADA para ser compatível com a nova struct bullet
bullet* pistol_shot(float x, float y, DirecaoTiro direcao, pistol *p) {
    bullet *lista_antiga = p->shots;
    bullet *novo_tiro = (bullet*)malloc(sizeof(bullet));

    if (!novo_tiro) {
        fprintf(stderr, "Falha ao alocar memória para um novo tiro!\n");
        return lista_antiga;
    }

    novo_tiro->x = x;
    novo_tiro->y = y;
    novo_tiro->vel_x = 0; // Inicializa as velocidades
    novo_tiro->vel_y = 0;

    // Define a velocidade com base na direção do tiro
    switch (direcao) {
        case DIR_DIREITA:
            novo_tiro->vel_x = PLAYER_BULLET_SPEED;
            break;
        case DIR_ESQUERDA:
            novo_tiro->vel_x = -PLAYER_BULLET_SPEED;
            break;
        case DIR_CIMA:
            novo_tiro->vel_y = -PLAYER_BULLET_SPEED;
            break;
    }

    novo_tiro->next = lista_antiga;
    p->shots = novo_tiro; // Atualiza o ponteiro da pistola
    
    return p->shots;
}

void pistol_destroy(pistol *p) {
    bullet *sentinel;
    for (bullet *index = p->shots; index != NULL; index = sentinel){
        sentinel = (bullet*) index->next;
        bullet_destroy(index);
    }
    free(p);
}
#define BULLET_SPEED 7.0f



// Atira reto para a esquerda ou direita
bullet* pistol_shot_horizontal(pistol *p, float start_x, float start_y, bool para_esquerda) {
    bullet *novo_tiro = (bullet*)malloc(sizeof(bullet));
    if (!novo_tiro) return NULL;

    novo_tiro->x = start_x;
    novo_tiro->y = start_y;
    novo_tiro->vel_y = 0; // Tiro reto
    novo_tiro->vel_x = para_esquerda ? -BULLET_SPEED : BULLET_SPEED;

    // Adiciona o novo tiro à lista da pistola
    novo_tiro->next = p->shots;
    p->shots = novo_tiro;

    return p->shots;
}

// Atira em direção a um ponto (x, y)
bullet* pistol_shot_direcionado(pistol *p, float start_x, float start_y, float target_x, float target_y) {
    bullet *novo_tiro = (bullet*)malloc(sizeof(bullet));
    if (!novo_tiro) return NULL;

    novo_tiro->x = start_x;
    novo_tiro->y = start_y;

    float dx = target_x - start_x;
    float dy = target_y - start_y;
    float distancia = sqrt(dx * dx + dy * dy);

    if (distancia > 0) {
        novo_tiro->vel_x = (dx / distancia) * BULLET_SPEED;
        novo_tiro->vel_y = (dy / distancia) * BULLET_SPEED;
    } else {
        novo_tiro->vel_x = BULLET_SPEED;
        novo_tiro->vel_y = 0;
    }
    
    novo_tiro->next = p->shots;
    p->shots = novo_tiro;
    
    return p->shots;
}

