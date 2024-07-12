#ifndef ARCHER
#define ARCHER

#include "mensagem.h"

struct arch{

	Persona* body;
	int flechas;

};


Archer* novo_archer(int, int);
void atirar_flecha(Archer*, Mensagem*);
void atualizar_archer(Archer*, Mensagem*);
void apagar_archer(Archer*);
#endif
