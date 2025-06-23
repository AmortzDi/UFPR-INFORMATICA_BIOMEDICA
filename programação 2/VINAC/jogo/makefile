# Parâmetros de compilação para exibir todos os avisos e incluir informações de depuração.
parametrosCompilacao = -Wall -g

# Nome do programa executável final. 
NOME_PROGRAMA = jogo

# Lista de todos os arquivos fonte .c do projeto.
FONTES = codigo.c Pistol.c Bullet.c jogo.c


OBJETOS = $(FONTES:.c=.o)


ALLEGRO_LIBS = $(shell pkg-config --libs --cflags allegro-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_main-5 allegro_ttf-5 allegro_dialog-5) -lm


all: $(NOME_PROGRAMA)


$(NOME_PROGRAMA): $(OBJETOS)
	gcc -o $@ $^ $(parametrosCompilacao) $(ALLEGRO_LIBS)


%.o: %.c
	gcc -c $< -o $@ $(parametrosCompilacao)


clean:
	rm -f *.o $(NOME_PROGRAMA)