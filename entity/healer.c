#include "healer.h"
#include <stdlib.h>


Healer* novo_healer(int x, int y){
	Healer* heal = malloc(sizeof(*heal));

	heal->body = nova_persona(x, y);
	heal->mana = 3;

	return heal;
}


void proteger_aliado(Healer* heal, Mensagem* msg){}


void atualizar_healer(Healer* heal, Mapa* map, Mensagem* msg){}


void apagar_healer(Healer* heal){
	if(heal == NULL){
		return;
	}

	apagar_persona(heal->body);
	free(heal);
}
