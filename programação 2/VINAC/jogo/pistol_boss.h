// Conteúdo do arquivo pistol_boss.h
#ifndef __PISTOL_BOSS__
#define __PISTOL_BOSS__

#include "Pistol.h"   // Inclui a definição OFICIAL de pistol e bullet
#include <stdbool.h>  // Para usar o tipo bool

// Protótipos das funções de tiro específicas do BOSS
bullet* pistol_shot_horizontal(pistol *p, float start_x, float start_y, bool para_esquerda);
bullet* pistol_shot_direcionado(pistol *p, float start_x, float start_y, float target_x, float target_y);

#endif