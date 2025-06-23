// Conteúdo do arquivo Pistol.h
#ifndef __PISTOL__
#define __PISTOL__
#include <stdbool.h>
#include "Bullet.h" // Garanta que está incluindo o arquivo correto

typedef struct {
    bullet *shots;
    int timer;
} pistol;

pistol* pistol_create();
bullet* pistol_shot(float x, float y, DirecaoTiro direcao, pistol *p);
void pistol_destroy(pistol *p);
bullet* pistol_shot_horizontal(pistol *p, float start_x, float start_y, bool para_esquerda);
bullet* pistol_shot_direcionado(pistol *p, float start_x, float start_y, float target_x, float target_y);

#endif