#include "archer.h"
#include "gameMaster.h"
#include "mapa.h"
#include <stdio.h>
#include <stdlib.h>


/* criarArq(int, int)
 * Aloca memoria e retorna um ponteiro para struct Archer
*/

Archer* criarArq(int x, int y){

	Archer* arq = malloc(sizeof(*arq));
	
	arq->body = nova_persona(x, y);
	arq->flechas = 3;

	return arq;
}




/* atirar(Archer*, Mensagem*)
 * Recebe ponteiro para uma struct Archer, e, caso possua flechas
 * atira uma
*/

void atirar(Archer* arq, Mensagem* msg){
	mudarDirecao(arq);
	arq->flechas--;

	msg->acao = 'a';
	msg->x = arq->body->dir[0];
	msg->y = arq->body->dir[1];
}


/* validarFlecha(Mapa*, int, int)
 * Checa se flecha já foi coletada por inimigo
 * Retorna 1 se a flecha está no chão e 0 caso tenha sido coletada
*/

int validarFlecha(Mapa* mapa, int x, int y){
	for(int i = 0; i < 3; i++){
		if(mapa->flechasColetadas[i][1] == x && mapa->flechasColetadas[i][2] == y){
			return 0;
		}
	}

	return 1;
}


/* coletarFlecha(Archer*, Mapa*, Mensagem*)
 * Coleta uma flecha no chao caso mochila nao esteja cheia e esteja proximo
*/

void coletarFlecha(Archer* arq, Mapa* mapa, Mensagem* msg){
	if(mapa->pos[arq->y][arq->x] > 0 && validarFlecha(mapa, arq->x, arq->y)){
		if(arq->flechas < 3){
			arq->flechas++;
			mapa->pos[arq->y][arq->x]--;
			msg->acao = 'c';
		}
	}
}


/* atualizar(Archer*)
 * Atualiza todas as propriedades de um Archer
*/

void atualizarArq(Archer* arq, Mapa* mapa, Mensagem* msg){
	atualizarAcao(arq);

	switch(arq->body->acao){
		case 'm':
			mover(arq, msg);
			coletarFlecha(arq, mapa, msg);
			break;
		case 'a':
			atirar(arq, msg);
			registrarFlechaLocal(arq, mapa);
			break;
		default:
			printf("Ação inválida\n");
			atualizarArq(arq, mapa, msg);
	}
}


/* apagarArq(Archer*)
 * Libera memória alocada ao utilizar a função 'criarArq(int, int)'
*/ 

void apagarArq(Archer* arq){
	if(arq == NULL){
		return;
	}

	apagar_persona(arq->body);
	free(arq);
}
