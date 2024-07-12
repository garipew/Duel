#ifndef MAP
#define MAP
#include "mensagem.h"
#define MAXWIDTH 16
#define MAXHEIGHT 16

struct mapa{

	int pos[MAXHEIGHT][MAXWIDTH];
	int flechasColetadas[3][3];

};


Mapa* criarMapa();
void atualizarMapa(Mapa*, Archer*);
void atualizarMapaServer(Mapa*, Archer*, Mensagem*);
void registrarFlechaColetada(Mapa*, int, int);
void desenharMapa(Mapa*);
void apagarMapa(Mapa*);
#endif
