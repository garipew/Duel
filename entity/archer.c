#include "archer.h"
#include "gameMaster.h"
#include "mapa.h"
#include <stdio.h>
#include <stdlib.h>


/* novo_archer(int, int)
 * Aloca memoria e retorna um ponteiro para struct Archer
*/
Archer* novo_archer(int x, int y){
	Archer* arq = malloc(sizeof(*arq));
	
	arq->body = nova_persona(x, y);
	arq->flechas = 3;

	return arq;
}


/* atirar(Archer*, Mensagem*)
 * Recebe ponteiro para uma struct Archer, e, caso possua flechas
 * atira uma
 * retorna -1 caso nao seja possivel atirar flecha
*/
int atirar_flecha(Archer* arq, Mensagem* msg){
	mudarDirecao(arq->body);
	if(--arq->flechas <= 0){
		arq->flechas++;
		return -1;
	}

	msg->acao[0] = 'a';
	msg->x[0] = arq->body->dir[0];
	msg->y[0] = arq->body->dir[1];
	return 0;
}


/* validarFlecha(Mapa*, int, int)
 * Checa se flecha já foi coletada por inimigo
 * Retorna 1 se a flecha está no chão e 0 caso tenha sido coletada
*/
int validarFlecha(Mapa* mapa, int x, int y){
}


/* coletarFlecha(Archer*, Mapa*, Mensagem*)
 * Coleta uma flecha no chao caso mochila nao esteja cheia e esteja proximo
*/
void coletarFlecha(Archer* arq, Mapa* mapa, Mensagem* msg){
}


/* atualizar_acao(Archer*)
 * Atualiza proxima açao de um Archer
*/
void atualizar_acao(Archer* arch){
	char acao[4];

	printf("\t\tArcher\n");
	printf("Escolha a proxima ação, digite [m] para mover ou [a] para atirar.\n");
	
	do{
		printf("Sua escolha: ");
		fgets(acao, 4, stdin);
		sscanf(acao, "%c", &(per->acao));
	} while(per->acao != 'm');
}


/* atualizar_archer(Archer*, Mensagem*)
 * Atualiza todas as propriedades de um Archer
*/
void atualizar_archer(Archer* arq, Mensagem* msg){
	int status = -1;
	do{
		atualizar_acao(arq);
		if(arq->body->acao == 'm'){
			status = mover(arq->body, msg);
			break;
		}
		if(arq->body->acao == 'a'){
			status = atirar_flecha(arq, msg);
			break;
		}
	} while(status == -1);
}


/* apagar_archer(Archer*)
 * Libera memória alocada ao utilizar a função 'criarArq(int, int)'
*/ 
void apagar_archer(Archer* arq){
	if(arq == NULL){
		return;
	}

	apagar_persona(arq->body);
	free(arq);
}
