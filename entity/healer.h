#ifndef HEALER
#define HEALER

#include "mensagem.h"

struct healer{

	Persona* body;
	int mana;

};


Healer* novo_healer(int, int);
void proteger_aliado(Healer*, Mensagem*);
void atualizar_healer(Healer*, Mapa*, Mensagem*);
void apagar_healer(Healer*);
#endif
