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
	
	arq->x = x;
	arq->y = y;
	arq->flechas = 3;
	arq->dir[0] = 0;
	arq->dir[1] = 0;
	arq->acao = ' ';	

	return arq;
}


/* atualizarDistancia(Archer*)
 * Lê do usuário a distância que deseja se mover.
*/

void atualizarDistancia(Archer* arq){
	
	char dist[10];
	int distancia;
	
	printf("Distancia: ");
	fgets(dist, 10, stdin);
	sscanf(dist, "%d", &distancia);
		
	if(distancia < 1){
		distancia = 1;
	} else if(distancia > 3){
		distancia = 3;
	}


	arq->dir[0] *= distancia;
	arq->dir[1] *= distancia;

}


/* mudarDirecao(Archer*)
 * Recebe ponteiro para uma struct Archer e atualiza sua direcao
*/

void mudarDirecao(Archer* arq){
	
	char dir[10];
	
	do{
		
		printf("Direcao: ");
		fgets(dir, 10, stdin);

		arq->dir[0] = 0;
		arq->dir[1] = 0;
	
		if(dir[0] == 'n'){
			arq->dir[1] = -1;
		} else if(dir[0] == 's'){
			arq->dir[1] = 1;
		}

		if(dir[1] == 'w'){
			arq->dir[0] = -1;
		} else if(dir[1] == 'e'){
			arq->dir[0] = 1;
		}


	} while(arq->dir[0] == 0 && arq->dir[1] == 0);
	

}


/* mover(Archer*, Mensagem*)
 * Recebe ponteiro para uma struct Archer e atualiza sua posicao
*/

void mover(Archer* arq, Mensagem* msg){
	
	mudarDirecao(arq);
	atualizarDistancia(arq);
	arq->x += arq->dir[0];
	arq->y += arq->dir[1];
	
	if(arq->x > MAXWIDTH - 1){
		arq->x = MAXWIDTH - 1;
	} else if(arq->x < 0){
		arq->x = 0;
	}

	if(arq->y > MAXHEIGHT - 1){
		arq->y = MAXHEIGHT - 1;
	} else if(arq->y < 0){
		arq->y = 0;
	}

	msg->acao = 'm';
	msg->x = arq->x;
	msg->y = arq->y;

}


/* atirar(Archer*, Mensagem*)
 * Recebe ponteiro para uma struct Archer, e, caso possua flechas
 * atira uma
*/

void atirar(Archer* arq, Mensagem* msg){

	mudarDirecao(arq);
	arq->flechas--;

	msg->acao = 'a';
	msg->x = arq->dir[0];
	msg->y = arq->dir[1];
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


/* atualizarAcao(Archer*)
 * Atualiza proxima açao de um Archer
*/

void atualizarAcao(Archer* arq){
	
	char acao[4];

	printf("\t\tSeu turno\n");
	printf("Escolha sua proxima ação, digite [m] para mover ou [a] para atirar.\n");
	
	do{
		printf("Sua escolha: ");
	
		fgets(acao, 4, stdin);
		sscanf(acao, "%c", &(arq->acao));
		
		if(arq->acao == 'a'){
			if(arq->flechas > 0){
				break;
			}
			printf("Flechas insuficientes.\n");
		}

	} while(arq->acao != 'm');

}


/* atualizar(Archer*)
 * Atualiza todas as propriedades de um Archer
*/

void atualizarArq(Archer* arq, Mapa* mapa, Mensagem* msg){
	
	atualizarAcao(arq);

	switch(arq->acao){
		
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

	free(arq);

}
