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
#include "jogo.h"

int main() {
    inicializar_allegro();

    ALLEGRO_DISPLAY *display = al_create_display(TELA_W, TELA_H);
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    if (!display || !timer || !queue) die("Falha ao criar display, timer ou queue.");
    
    al_set_window_title(display, "TERONA");
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());

    GameResources resources;
    load_resources(&resources);
    
    GameState estado_atual = ESTADO_MENU;
    while (estado_atual != ESTADO_SAIR) {
        switch (estado_atual) {
            case ESTADO_MENU:       estado_atual =executar_menu(queue, timer, &resources);       break;
            case ESTADO_JOGANDO:    estado_atual = executar_jogo(queue, timer, &resources);    break;
            case ESTADO_GAME_OVER:  estado_atual = executar_game_over(queue, timer, &resources);  break;
            case ESTADO_SAIR:       break;
        }
    }

    destroy_resources(&resources);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    return 0;
}
