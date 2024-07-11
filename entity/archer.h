#ifndef ARCHER
#define ARCHER

#include "mensagem.h"

struct arch{

	Persona* body;
	int flechas;

};


Archer* criarArq(int, int);
void atirar(Archer*, Mensagem*);
void atualizarArq(Archer*, Mapa*, Mensagem*);
void apagarArq(Archer*);
#endif
