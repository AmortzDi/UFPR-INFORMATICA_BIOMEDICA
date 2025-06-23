#include <stdlib.h>
#include "Bullet.h"



void bullet_move(bullet *lista_tiros) {
    // Itera por todos os tiros na lista
    for (bullet *b = lista_tiros; b != NULL; b = (bullet*)b->next) {
        // Simplesmente adiciona a velocidade à posição
        b->x += b->vel_x;
        b->y += b->vel_y;
    }
}
void bullet_destroy(bullet *element){																		//Implementação da função "bullet_destroy"

	free(element);																							//Libera a memória da instância de projétil
}