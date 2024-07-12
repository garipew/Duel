#include "mapa.h"
#include "archer.h"
#include "gameMaster.h"
#include <stdlib.h>
#include <stdio.h>


/* criarMapa()
 * Aloca memoria para um novo mapa e retorna ponteiro para
 * nova struct Mapa
*/

Mapa* criarMapa(){

	Mapa* mapa = malloc(sizeof(*mapa));
	
	for(int i = 0; i < MAXHEIGHT; i++){
		for(int j = 0; j < MAXWIDTH; j++){
			mapa->pos[i][j] = 0;
		}
	}

	return mapa;

}


/* revelarFlechaColetada(Mapa*, int, int)
 * Revela que flecha foi coletada por inimigo anteriormente
*/ 

void revelarFlechaColetada(Mapa* mapa, int x, int y){
	if(mapa->pos[y][x] > 0){
		mapa->pos[y][x]--;
	}
}


/* atualizarMapa(Archer*)
 * Atualiza posicoes no mapa
 * Jogador é registrado como -1.
 * Qualquer valor < -1 indica flechas na pos do Jogador.
 * Quantidade de flechas são armazenadas com valores entre -5 e 6 
 * Valores < -5 são especiais, player morto ou eco por exemplo
*/

void atualizarMapa(Mapa* mapa, Archer* arq){
	
	int xFlecha, yFlecha;	

	for(int i = 0; i < MAXHEIGHT; i++){
		for(int j = 0; j < MAXWIDTH; j++){
			if(mapa->pos[i][j] <= -20){
				mapa->pos[i][j] += 20;
				mapa->pos[i][j] *= -1;
				continue;
			}

			if(mapa->pos[i][j] < 0){
				mapa->pos[i][j]++;
				mapa->pos[i][j] *= -1;
			}
		}
	}

	mapa->pos[arq->y][arq->x] *= -1;
	mapa->pos[arq->y][arq->x]--;
	

	for(int k = 0; k < 3; k++){
		if(mapa->flechasColetadas[k][0] > 0){
			mapa->flechasColetadas[k][0]--;
		}

		if(mapa->flechasColetadas[k][0] == 0){
			revelarFlechaColetada(mapa, mapa->flechasColetadas[k][1], mapa->flechasColetadas[k][2]);
		}
	}	

}


/* atualizarMapaServer(Mapa*, Archer*, Archer*, Mensagem*)
 * Atualiza o mapa, usado apenas pelo servidor.
*/

void atualizarMapaServer(Mapa* mapa, Archer* arq1, Archer* arq2, Mensagem* msg){
	
	int xFlecha, yFlecha;	

	for(int i = 0; i < MAXHEIGHT; i++){
		for(int j = 0; j < MAXWIDTH; j++){
			
			if(mapa->pos[i][j] < 0){ 
				mapa->pos[i][j]++;
				mapa->pos[i][j] *= -1;
			}
		}
	}

	mapa->pos[arq1->y][arq1->x] *= -1;
	mapa->pos[arq1->y][arq1->x]--;
	
	mapa->pos[arq2->y][arq2->x] *= -1;
	mapa->pos[arq2->y][arq2->x]--;

}


/* registrarFlechaColetada(int, int)
 * Armazena coordenadas de uma flecha coletada pelo inimigo
 * Para revelar posteriormente.
*/

void registrarFlechaColetada(Mapa* mapa, int x, int y){
	for(int i = 0; i < 3; i++){
				
		if(mapa->flechasColetadas[i][0] != 0){
			continue;
		}
				
		mapa->flechasColetadas[i][0] = 3;
		mapa->flechasColetadas[i][1] = x;
		mapa->flechasColetadas[i][2] = y;
				
		break;
	}
}


/* desenharMapa(Archer*)
 * Desenha o mapa e o jogador
*/ 
void desenharMapa(Mapa* mapa){

	for(int i = 0; i < MAXHEIGHT; i++){
		for(int j = 0; j < MAXWIDTH; j++){
			
			if(mapa->pos[i][j] <= -20){
				printf("? ");
				continue;
			}

			if(mapa->pos[i][j] < -10){
				printf("# ");
				continue;
			}
			
			if(mapa->pos[i][j] < 0){
				printf("@ ");
			}else if(mapa->pos[i][j] == 0){
				printf(". ");
			} else{
				printf("x ");
			}


		}
		printf("\n");
	}

	printf("\n\n");
}


/* apagarMapa(Mapa*)
 * Libera memoria alocada na funcao 'criarMapa()'
*/

void apagarMapa(Mapa* mapa){
	
	if(mapa == NULL){
		return;
	}

	free(mapa);

}
