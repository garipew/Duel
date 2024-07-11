#ifndef PERSONA
#define PERSONA

#include "mensagem.h"

struct persona{

	int x,y;
	int dir[2];
	char acao;

}

Persona* nova_persona(int, int);
void mover(Persona*);
void apagar_persona(Persona*);
#endif
