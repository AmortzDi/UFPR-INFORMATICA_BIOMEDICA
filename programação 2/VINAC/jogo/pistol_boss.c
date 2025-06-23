#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "pistol_boss.h"

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

