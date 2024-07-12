#ifndef HUNTER
#define HUNTER

#include "mensagem.h"

struct hunter{

	Persona* body;
	int mana;

};


Hunter* novo_hunter(int, int);
void rastrear_inimigo(Hunter*, Mensagem*);
void atualizar_hunter(Hunter*, Mapa*, Mensagem*);
void apagar_hunter(Hunter*);
#endif
