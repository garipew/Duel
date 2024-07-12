#include "mapa.h"
#include "archer.h"
#include "gameMaster.h"
#include <stdlib.h>
#include <stdio.h>

#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))


/* criarMapa()
 * Aloca memoria para um novo mapa e retorna ponteiro para
 * nova struct Mapa
*/
Mapa* criarMapa(){
	Mapa* mapa = malloc(sizeof(*mapa));
	mapa->width = MAXWIDTH;
	mapa->height = MAXHEIGHT;
	memset(mapa->flechas, -1, sizeof(mapa->flechas))
	return mapa;
}


/* checar_posicao(Mapa*, int, int)
 * Checa se há uma flecha na posicao dada como parametro e retorna o caractere
 * que deve ser escrito
 */
char checar_posicao(Mapa* map, int y, int x){
	for(int i=0; i<6; i++){
		if(mapa->flechas[i][0] == x && mapa->flechas[i][1] == y){
			return 'x';
		}
	}
	return '.';
}


/* desenhar_mapa(Mapa*)
 * Desenha o mapa
*/
void desenhar_mapa(Mapa* map){
	for(int i=0; i<map->height; i++){
		for(int j=0; j<map->width; j++){
			printf("%c ", checar_posicao(map, i, j));
		}
		printf("\n");	
	}
}


/* registra_flecha(Mapa*, int, int)
 * Armazena a posicao de uma flecha no mapa
*/
void registrar_flecha(Mapa* mapa, int x, int y){
	for(int i = 0; i < 6; i++){
		if(mapa->flechas[i][0] != 0){
			continue;
		}
		mapa->flechas[i][1] = x;
		mapa->flechas[i][2] = y;
		break;
	}
}


/* atualizar_mapa(Mapa*, Persona*[])
 * Verifica se uma flecha foi coletada
*/
void atualizar_mapa(Mapa* map, Persona* pers[]){
	for(int i=0; i<ARRAYSIZE(flechas); i++){
		if(mapa->flechas[i][0] == pers[i % ARRAYSIZE(pers)]->x &&
				mapa->flechas[i][1] == pers[i% ARRAYSIZE(pers)]->y){
			mapa->flechas[i][0] = -1;
			mapa->flechas[i][1] = -1;
		}
	}
}


/* apagar_mapa(Mapa*)
 * Libera memoria alocada na funcao 'criarMapa()'
*/
void apagarMapa(Mapa* mapa){
	if(mapa == NULL){
		return;
	}

	free(mapa);
}


/* revelarFlechaColetada(Mapa*, int, int)
 * Revela que flecha foi coletada por inimigo anteriormente
*/ 
void revelarFlechaColetada(Mapa* mapa, int x, int y){
	if(mapa->pos[y][x] > 0){
		mapa->pos[y][x]--;
	}
}


/* registrarFlechaColetada(int, int)
 * Armazena coordenadas de uma flecha coletada pelo inimigo
 * Para revelar posteriormente.
*/
void registrarFlechaColetada(Mapa* mapa, int x, int y){
	for(int i = 0; i < 3; i++){
		if(mapa->flechas[i][0] != 0){
			continue;
		}
		mapa->flechas[i][0] = 3;
		mapa->flechas[i][1] = x;
		mapa->flechas[i][2] = y;
		break;
	}
}

