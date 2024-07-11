#include "persona.h"
#include <stdlib.h>


/* nova_persona(int, int)
 * Aloca espaço na memoria e retorna um ponteiro para struct Persona
*/ 
Persona* nova_persona(int x, int y){
	Persona* per = malloc(sizeof(*per));

	per->x = x;
	per->y = y;
	per->dir[0] = 0;
	per->dir[1] = 1;
	per->acao = ' ';

	return per;
}


/* atualizarDistancia(Persona*)
 * Lê do usuário a distância que deseja se mover.
*/
void atualizarDistancia(Persona* per){
	
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


	per->dir[0] *= distancia;
	per->dir[1] *= distancia;

}


/* mudarDirecao(Persona*)
 * Recebe ponteiro para uma struct Persona e atualiza sua direcao
*/
void mudarDirecao(Persona* per){
	
	char dir[10];
	
	do{
		
		printf("Direcao: ");
		fgets(dir, 10, stdin);

		per->dir[0] = 0;
		per->dir[1] = 0;
	
		if(dir[0] == 'n'){
			per->dir[1] = -1;
		} else if(dir[0] == 's'){
			per->dir[1] = 1;
		}

		if(dir[1] == 'w'){
			per->dir[0] = -1;
		} else if(dir[1] == 'e'){
			per->dir[0] = 1;
		}


	} while(per->dir[0] == 0 && per->dir[1] == 0);
	

}


/* mover(Persona*, Mensagem*)
 * Recebe ponteiro para uma struct Persona e atualiza sua posicao
*/
void mover(Persona* per, Mensagem* msg){
	mudarDirecao(per);
	atualizarDistancia(per);
	per->x += per->dir[0];
	per->y += per->dir[1];
	
	if(per->x > MAXWIDTH - 1){
		per->x = MAXWIDTH - 1;
	} else if(per->x < 0){
		per->x = 0;
	}

	if(per->y > MAXHEIGHT - 1){
		per->y = MAXHEIGHT - 1;
	} else if(per->y < 0){
		per->y = 0;
	}

	msg->acao = 'm';
	msg->x = per->x;
	msg->y = per->y;
}


/* atualizarAcao(Persona*)
 * Atualiza proxima açao de um Persona
*/
void atualizarAcao(Persona* per){
	char acao[4];

	printf("\t\tSeu turno\n");
	printf("Escolha sua proxima ação, digite [m] para mover ou [a] para atirar.\n");
	
	do{
		printf("Sua escolha: ");
	
		fgets(acao, 4, stdin);
		sscanf(acao, "%c", &(per->acao));
		
		if(per->acao == 'a'){
			if(per->flechas > 0){
				break;
			}
			printf("Flechas insuficientes.\n");
		}

	} while(per->acao != 'm');
}


/* apagar_persona(Persona*)
 * Libera memória alocada ao criar nova persona
*/
void apagar_persona(Persona* per){
	if(per == NULL){
		return;
	}
	free(per);
}
