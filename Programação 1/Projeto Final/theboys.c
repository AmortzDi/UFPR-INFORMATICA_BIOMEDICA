#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "lef.h"
#include "fila.h"
#include "conjunto.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 6)
#define N_MISSOES 100
#define MENOR_DISTANCIA 1.0e10

/*define para utilizacao do switch*/
#define CHEGADA 0
#define ESPERA 1
#define DESISTE 2
#define AVISA 3
#define ENTRADA 4
#define SAIDA 5
#define VIAGEM 6
#define MISSAO 7
#define FIM 8


/* funcoes que voce ache necessarias aqui */
struct herois {
    int id_herois;
    struct conjunto *habilidades;
    int paci;
    int velocidade;
    int exp;
    int base;
};

struct base {
    int id_base;
    int lotacao;
    struct conjunto *herois_presentes;
    struct fila *fila_espera;
    int pos_bx;
    int pos_by;  
};

struct missao {
    int id_missao;
    struct conjunto *habilidade_missao;
    int pos_mx;
    int pos_my;
};

struct mundo {
    struct herois herois_mundo[N_HEROIS];
    struct base base_mundo[N_BASES];
    struct missao missao_mundo[N_MISSOES];
    int nherois;
    int nbases;
    int nmissoes;
    int nhabilidades;
    int tam_mundo;
    int tempo;
};

struct distancia_base {
    double distancia;
    int id_base;
};

int aleat(int min, int max) {
    return min + rand() % (max - min + 1);
}
struct mundo cria_mundo() {
    struct mundo m;
    int aleatorio = 0;
    int i, k;

    m.nherois = N_HEROIS;
    m.nbases = N_BASES;
    m.nmissoes = N_MISSOES;
    m.tam_mundo = N_TAMANHO_MUNDO;
    m.tempo = 0;
    m.nhabilidades = N_HABILIDADES;

    for (i = 0; i < N_HEROIS; i++) {
        m.herois_mundo[i].id_herois = i;
        m.herois_mundo[i].exp = 0;
        m.herois_mundo[i].paci = aleat(0, 100);
        m.herois_mundo[i].velocidade = aleat(50, 5000);
        m.herois_mundo[i].habilidades = cria_cjt(N_HABILIDADES);
        m.herois_mundo[i].base = -1;

        aleatorio = aleat(1, 3);

        for (k = 0; k < aleatorio; k++) {
            int novo;
            do {
                novo = aleat(0, N_HABILIDADES - 1);
            } while (pertence_cjt(m.herois_mundo[i].habilidades, novo));
            insere_cjt(m.herois_mundo[i].habilidades, novo);
        }
    }

    for (i = 0; i < N_MISSOES; i++) {
        m.missao_mundo[i].id_missao = i;
        m.missao_mundo[i].pos_mx = aleat(0, N_TAMANHO_MUNDO - 1);
        m.missao_mundo[i].pos_my = aleat(0, N_TAMANHO_MUNDO - 1);
        m.missao_mundo[i].habilidade_missao = cria_cjt(N_HABILIDADES);

        aleatorio = aleat(6, 10);

        for (k = 0; k < aleatorio; k++) {
            int novo;
            do {
                novo = aleat(0, N_HABILIDADES - 1);
            } while (pertence_cjt(m.missao_mundo[i].habilidade_missao, novo));
            insere_cjt(m.missao_mundo[i].habilidade_missao, novo);
        }
    }

    for (i = 0; i < N_BASES; i++) {
        m.base_mundo[i].id_base = i;
        m.base_mundo[i].pos_bx = aleat(0, N_TAMANHO_MUNDO - 1);
        m.base_mundo[i].pos_by = aleat(0, N_TAMANHO_MUNDO - 1);
        m.base_mundo[i].lotacao = aleat(3, 10);
        m.base_mundo[i].herois_presentes = cria_cjt(m.base_mundo[i].lotacao);
        m.base_mundo[i].fila_espera = fila_cria(); 
    }

    return m;
}

void destroi_mundo(struct mundo *m) {
    int i;

    for (i = 0; i < N_HEROIS; i++) 
        destroi_cjt(m->herois_mundo[i].habilidades);
    

    for (i = 0; i < N_MISSOES; i++) 
        destroi_cjt(m->missao_mundo[i].habilidade_missao);
    

    for (i = 0; i < N_BASES; i++) {
        destroi_cjt(m->base_mundo[i].herois_presentes);
        fila_destroi(&m->base_mundo[i].fila_espera);
    }
}
/* retira o heroi do conjunto de presentes da base e retorna uma base aleatoria */
int evento_sai(int tempo, struct mundo *m, int h, int b) {
    int id_base_aleat;
    
    retira_cjt(m->base_mundo[b].herois_presentes,m->herois_mundo[h].id_herois);

    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", tempo, m->herois_mundo[h].id_herois,
             m->base_mundo[b].id_base, cardinalidade_cjt(m->base_mundo[b].herois_presentes),
              m->base_mundo[b].lotacao);

    /* define uma base destino aleatoria */
    /* um do while para que o heroi nao va para a mesma base */
    do 
        id_base_aleat = aleat(0, N_BASES - 1);
    while (id_base_aleat == m->base_mundo[b].id_base);

    return id_base_aleat;
}

/* calcula o TPB e retorna o valor */
int evento_entra(int tempo, struct mundo *m, int h, int b) {
    int TPB;

    TPB = 15 + (m->herois_mundo[h].paci * aleat(1, 20));

    printf ("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", tempo, m->herois_mundo[h].id_herois,
             m->base_mundo[b].id_base, cardinalidade_cjt(m->base_mundo[b].herois_presentes), 
             m->base_mundo[b].lotacao, tempo + TPB);

    return TPB;
}

void evento_avisa(int tempo, struct mundo *m, int b) {
    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ", tempo, m->base_mundo[b].id_base, 
            cardinalidade_cjt(m->base_mundo[b].herois_presentes), m->base_mundo[b].lotacao);
    imprime_cjt(m->base_mundo[b].herois_presentes);
    return;
}
 /* adiciona o ID do heroi na fila de espera da base */
void evento_espera(int tempo, struct mundo *m, int h, int b) {
   
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, m->herois_mundo[h].id_herois,
            m->base_mundo[b].id_base, fila_tamanho(m->base_mundo[b].fila_espera));
   
    enqueue(m->base_mundo[b].fila_espera, m->herois_mundo[h].id_herois);
}

/* calcula a distancia do heroi ate a outra base e a retorna */
int evento_viaja(int tempo, struct mundo *m ,int h, int b_d ) {
    /*Obtemos as posições das bases de origem e destino*/
    int pos_x_origem = m->base_mundo[m->herois_mundo[h].base].pos_bx;
    int pos_y_origem = m->base_mundo[m->herois_mundo[h].base].pos_by;
    int pos_x_destino = m->base_mundo[b_d].pos_bx;
    int pos_y_destino = m->base_mundo[b_d].pos_by;

    /* Calcula a distância cartesiana*/
    double distancia_x = pow(pos_x_destino - pos_x_origem, 2);
    double distancia_y = pow(pos_y_destino - pos_y_origem, 2);
    int distancia = sqrt(distancia_x + distancia_y);

    int duracao = (int)ceil(distancia / m->herois_mundo[h].velocidade);

    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, m->herois_mundo[h].id_herois,
            m->herois_mundo[h].base,b_d, distancia,
            m->herois_mundo[h].velocidade, tempo + duracao);
   
    /* retorna o tempo de duracao da viajem */
    return duracao;
}

 /* define uma base destino aleatoria*/
int evento_desiste(int tempo,struct mundo *m, int h, int b) {
    int base_destino;

    printf ("%6d: DESISTE HEROI %2d BASE %d\n", tempo, m->herois_mundo[h].id_herois,
            m->base_mundo[b].id_base);

    base_destino = aleat(0, N_BASES - 1);

    return base_destino;
}
/*Evento CHEGA
Representa um herói H chegando em uma base B no instante T. Ao chegar, o herói analisa o tamanho da fila e decide se espera para entrar ou desiste:
*/
int evento_chega(int tempo, struct mundo *m, int h, int b) {
    int vagas,fila_espera_vazia;
    int espera = 0;

    m->herois_mundo[h].base = m->base_mundo[b].id_base;

    
    /*Verifica se há vagas na base*/
    vagas = m->base_mundo[b].lotacao > cardinalidade_cjt(m->base_mundo[b].herois_presentes);

    /*Verifica se a fila de espera está vazia*/
    fila_espera_vazia = fila_vazia(m->base_mundo[b].fila_espera);


    /*Determina se o herói decide esperar ou desiste*/
    if (vagas && fila_espera_vazia) 
        espera = 1; /*O herói espera se há vagas e a fila está vazia*/
    else if (m->herois_mundo[h].paci > (10 * fila_tamanho(m->base_mundo[b].fila_espera))) 
        espera = 1; 
     else 
        espera = 0; 
    
    if(espera){
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA \n", tempo, m->herois_mundo[h].id_herois, 
                m->base_mundo[b].id_base, cardinalidade_cjt(m->base_mundo[b].herois_presentes), m->base_mundo[b].lotacao);
        return 1;
    }else{
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE \n", tempo, m->herois_mundo[h].id_herois, 
                m->base_mundo[b].id_base, cardinalidade_cjt(m->base_mundo[b].herois_presentes), m->base_mundo[b].lotacao);
        return 0;
    }
}
int comparar(const void* a, const void* b) {
    return (*(struct distancia_base*)a).distancia - (*(struct distancia_base*)b).distancia;
}
int evento_missao(int tempo, int indice_missao, struct mundo *m) {
    int i, h, b;
    double distancia[N_BASES];
    struct distancia_base distanciasBases[N_BASES];

    /* Calcula as distâncias das bases para a missão */
    for(i = 0; i < N_BASES; i++) {
        distancia[i] = sqrt(pow(m->base_mundo[i].pos_bx - m->missao_mundo[indice_missao].pos_mx, 2) +
                            pow(m->base_mundo[i].pos_by - m->missao_mundo[indice_missao].pos_my, 2));
        distanciasBases[i].distancia = distancia[i];
        distanciasBases[i].id_base = i;
    }

    /* Ordena as bases com base nas distâncias usando qsort */
    qsort(distanciasBases, N_BASES, sizeof(struct distancia_base), comparar);

    printf("%6d: MISSAO %d TENT %d HAB REQ: ", tempo, m->missao_mundo[indice_missao].id_missao, 1);
    imprime_cjt(m->missao_mundo[indice_missao].habilidade_missao);

    bool achou = false;
    int base_localizada = -1;

    /* Procura a base mais próxima com heróis que têm as habilidades necessárias */
    for (b = 0; b < N_BASES && !achou; b++) {
        struct conjunto *c_habilidades_base = cria_cjt(N_HABILIDADES);

        for (h = 0; h < N_HEROIS; h++) {
            if (pertence_cjt(m->base_mundo[distanciasBases[b].id_base].herois_presentes,
                            m->herois_mundo[h].id_herois)) {
                
                struct conjunto *temp = uniao_cjt(c_habilidades_base, m->herois_mundo[h].habilidades);
                destroi_cjt(c_habilidades_base);
                c_habilidades_base = temp;
            }
        }

        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS: ", tempo, m->missao_mundo[indice_missao].id_missao,
                m->base_mundo[distanciasBases[b].id_base].id_base, (int)distanciasBases[b].distancia);
        imprime_cjt(m->base_mundo[distanciasBases[b].id_base].herois_presentes);

        printf("%6d: MISSAO %d HAB HEROI ", tempo, m->missao_mundo[indice_missao].id_missao);
        for (h = 0; h < N_HEROIS; h++) {
            if (pertence_cjt(m->base_mundo[distanciasBases[b].id_base].herois_presentes, m->herois_mundo[h].id_herois)) {
                printf("%2d: ", m->herois_mundo[h].id_herois);
                imprime_cjt(m->herois_mundo[h].habilidades);
            }
        }

        printf("%6d: MISSAO %d UNIAO HAB BASE %d: ", tempo, m->missao_mundo[indice_missao].id_missao,
               m->base_mundo[distanciasBases[b].id_base].id_base);
        imprime_cjt(c_habilidades_base);

        if (contido_cjt(c_habilidades_base, m->missao_mundo[indice_missao].habilidade_missao)) {
            base_localizada = distanciasBases[b].id_base;
            achou = true;
        }

        destroi_cjt(c_habilidades_base);
    }

    /* Incrementa o xp dos heróis que estão na base apta */
    if (achou) {
        for (i = 0; i < N_HEROIS; i++) {
            if (pertence_cjt(m->base_mundo[base_localizada].herois_presentes, m->herois_mundo[i].id_herois)) 
                m->herois_mundo[i].exp += 1;      
        }

        printf("%6d: MISSAO %d CUMPRIDA BASE %d HEROIS: ", tempo, m->missao_mundo[indice_missao].id_missao,
               m->base_mundo[base_localizada].id_base);
        imprime_cjt(m->base_mundo[base_localizada].herois_presentes);
        return 1;
    }

    printf("%6d: MISSAO %d IMPOSSIVEL \n", tempo, m->missao_mundo[indice_missao].id_missao);
    return 0;
}

void evento_fim(int tempo, struct mundo *m, int missao_concluida, int missao_adiada){
    int n_missoes = N_MISSOES;
    int i;
   
    printf("%6d: FIM \n", tempo);

  /*imprime os herois*/
    for (i = 0;  i < N_HEROIS; i++){
         printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS ", m->herois_mundo[i].id_herois,m->herois_mundo[i].paci, 
                m->herois_mundo[i].velocidade, m->herois_mundo[i].exp);
         imprime_cjt(m->herois_mundo[i].habilidades);
       }
   
   printf("%d/%d MISSOES CONCLUIDAS  (%.2f%%) MEDIA %.2F TENTATIVAS/MISSAO \n", missao_concluida, N_MISSOES, ((double)missao_concluida/n_missoes)*100.0, ( (double)missao_concluida+(double)missao_adiada)/n_missoes);
    
    return;
}


int main() {
    int espera_ou_desiste = 0, tpb = 0;
    int destino = 0, duracao = 0, aux = 0;
    int concluida = 0, missao_concluida = 0, missao_adiada = 0;
    int i;

    srand(0);

    struct lef_t *simulacao = cria_lef();
    struct evento_t *evento;
    struct evento_t *novo_evento;
    struct mundo m = cria_mundo();  

    /*Adiciona eventos iniciais para a chegada de heróis*/
    for (i = 0; i < N_HEROIS; i++) {
        int tmpo = aleat(1, 4321);
        evento = cria_evento(tmpo, 0, m.herois_mundo[i].id_herois, m.base_mundo[aleat(0, N_BASES-1)].id_base);
        insere_lef(simulacao, evento);
    }

    /*Adiciona eventos para as missões*/
    for (i = 0; i < N_MISSOES; i++) {
        int tmpo = aleat(0, T_FIM_DO_MUNDO);
        evento = cria_evento(tmpo, 7, m.missao_mundo[i].id_missao, 0);
        insere_lef(simulacao, evento);
    }

    /*Adiciona evento final para o fim da simulação*/
    evento = cria_evento(T_FIM_DO_MUNDO, FIM, 0, 0);
    insere_lef(simulacao, evento);

    while (!vazia_lef(simulacao)) {
        evento = retira_lef(simulacao);
        m.tempo = evento->tempo;

        switch (evento->tipo) {
            case CHEGADA: /* Evento de chegada*/
                espera_ou_desiste = evento_chega(evento->tempo, &m, evento->dado1, evento->dado2);
                if (espera_ou_desiste == 1) {
                    novo_evento = cria_evento(m.tempo, ESPERA, evento->dado1, evento->dado2);
                    insere_lef(simulacao, novo_evento);
                } else {
                    novo_evento = cria_evento(m.tempo, DESISTE, evento->dado1, evento->dado2);
                    insere_lef(simulacao, novo_evento);
                }
                break;

            case ESPERA: /*Evento de espera*/
                evento_espera(evento->tempo, &m, evento->dado1, evento->dado2);
                novo_evento = cria_evento(m.tempo, AVISA, evento->dado1, evento->dado2);
                insere_lef(simulacao, novo_evento);
                break;

            case DESISTE: /*Evento de desistência*/
                destino = evento_desiste(evento->tempo, &m, evento->dado1, evento->dado2);
                novo_evento = cria_evento(m.tempo, VIAGEM, evento->dado1, destino);
                insere_lef(simulacao, novo_evento);
                break;
            case AVISA: /*Evento de aviso*/
                evento_avisa(evento->tempo, &m, evento->dado2);
                int heroiRemovido;
                while (m.base_mundo[evento->dado2].lotacao > cardinalidade_cjt(m.base_mundo[evento->dado2].herois_presentes)
                       && fila_tamanho(m.base_mundo[evento->dado2].fila_espera) != 0) {
                    dequeue(m.base_mundo[evento->dado2].fila_espera,&heroiRemovido );
                    printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d \n", evento->tempo, m.base_mundo[evento->dado2].id_base, heroiRemovido);
                    insere_cjt(m.base_mundo[evento->dado2].herois_presentes, heroiRemovido);
                    novo_evento = cria_evento(m.tempo, ENTRADA, heroiRemovido, evento->dado2);
                    insere_lef(simulacao, novo_evento);
                }
                break;
            case ENTRADA: /*Evento de entrada*/
                tpb = evento_entra(evento->tempo, &m, evento->dado1, evento->dado2);
                novo_evento = cria_evento(m.tempo + tpb, SAIDA, evento->dado1, evento->dado2);
                insere_lef(simulacao, novo_evento);
                break;
            case SAIDA: /*Evento de saída*/
                aux = evento->dado2;
                destino = evento_sai(evento->tempo, &m, evento->dado1, evento->dado2);
                novo_evento = cria_evento(m.tempo, VIAGEM, evento->dado1, destino);
                insere_lef(simulacao, novo_evento);
                novo_evento = cria_evento(m.tempo, AVISA, evento->dado1, aux);
                insere_lef(simulacao, novo_evento);
                break;
            case VIAGEM: /*Evento de viagem*/
                duracao = evento_viaja(evento->tempo, &m, evento->dado1, evento->dado2);
                novo_evento = cria_evento(m.tempo + duracao, CHEGADA, evento->dado1, evento->dado2);
                insere_lef(simulacao, novo_evento);
                break;
            case MISSAO: /*Evento de missão*/
                concluida = evento_missao(evento->tempo, evento->dado1, &m);
                if (concluida == 1) {
                    missao_concluida++;
                } else {
                    missao_adiada++;
                    novo_evento = cria_evento(m.tempo + 24 * 60, MISSAO, evento->dado1, 0);
                    insere_lef(simulacao, novo_evento);
                }
                break;
            case FIM: /*Evento de fim*/
                evento_fim(evento->tempo, &m, missao_concluida, missao_adiada);
                destroi_evento(evento);
                destroi_lef(simulacao);
                destroi_mundo(&m);
                return 0;
        }
        destroi_evento(evento);
    }
    return 0;
}
