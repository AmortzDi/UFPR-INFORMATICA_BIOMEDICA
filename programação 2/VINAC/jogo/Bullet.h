// Conteúdo do arquivo Bullet.h
#ifndef __BULLET__
#define __BULLET__

typedef enum { DIR_DIREITA, DIR_ESQUERDA, DIR_CIMA } DirecaoTiro;

typedef struct bullet {
    float x, y;
    float vel_x, vel_y; // A chave da flexibilidade!
    struct bullet *next;
} bullet;

void bullet_move(bullet *lista_de_tiros);
void bullet_destroy(bullet *tiro);

#endif