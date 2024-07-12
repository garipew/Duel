#include "hunter.h"
#include <stdlib.h>


Hunter* novo_hunter(int x, int y){
	Hunter* hunt = malloc(sizeof(*hunt));

	hunt->body = nova_persona(x, y);
	hunt->mana = 3;

	return hunt;
}


void rastrear_inimigo(Hunter* hunt, Mensagem* msg){}


void atualizar_hunter(Hunter* hunt, Mapa* map, Mensagem* msg){}


void apagar_hunter(Hunter* hunt){
	if(hunt == NULL){
		return;
	}

	apagar_persona(hunt->body);
	free(hunt);
}
