#ifndef MAP
#define MAP
#include "mensagem.h"
#define MAXWIDTH 16
#define MAXHEIGHT 16

struct mapa{
	int width;
	int height;
	int flechas[6][2];
};


Mapa* criarMapa();
void atualizar_mapa(Mapa*, Persona*[]);
void desenhar_mapa(Mapa*);
void registrar_flecha(mapa*, int, int);
void registrarFlechaColetada(Mapa*, int, int);
void apagar_mapa(Mapa*);
#endif
