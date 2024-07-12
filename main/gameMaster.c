#include "gameMaster.h"
#include "mapa.h"
#include "archer.h"
#include "mensagem.h"
#include "connection.h"
#include <stdlib.h>
#include <stdio.h>


#define MAXBUFFSIZE 6
#define MAXMSGSIZE 6



/* registrarFlecha(Archer*, Mapa*, Mensagem*)
 * Encontra a posicao final de uma flecha disparada
 * Checa se flecha atinge alguem
 * Prepara mensagem para enviar
 * Usado pelo servidor
*/ 
void registrarFlecha(Archer* arq, Mapa* mapa, Mensagem* msg){

	int xFlecha = arq->x;
	int yFlecha = arq->y;

	for(int i = 0; i < MAXHEIGHT; i++){
			
		xFlecha += msg->x;
		yFlecha += msg->y;

		if(xFlecha < 0){
			xFlecha = 0;
			yFlecha -= msg->y;
			break;
		}

		if(xFlecha > MAXWIDTH - 1){
			xFlecha = MAXWIDTH - 1;
			yFlecha -= msg->y;
			break;
		}
			
		if(yFlecha < 0){
			yFlecha = 0;
			xFlecha -= msg->x;
			break;
		}

		if(yFlecha > MAXHEIGHT - 1){
			yFlecha = MAXHEIGHT - 1;
			xFlecha -= msg->x;
			break;
		}

		if(mapa->pos[yFlecha][xFlecha] < 0 && mapa->pos[yFlecha][xFlecha] > -20 && msg != NULL){
			msg->acao = 'l';
			break;
			// Acertou alguem.
		}	
	}

	if(msg == NULL){
		return;
	}
	msg->x = xFlecha;
	msg->y = yFlecha;
	if(msg->acao != 'l'){
		mapa->pos[yFlecha][xFlecha]++;
		msg->acao = 'f';
	}

}


/* registrarFlechaLocal(Archer*, Mapa*)
 * Calcula trajetoria de uma flecha localmente
 * Usando pelos jogadores
*/

void registrarFlechaLocal(Archer* arq, Mapa* mapa){

	int xFlecha = arq->x;
	int yFlecha = arq->y;

	for(int i = 0; i < MAXHEIGHT; i++){
			
		xFlecha += arq->dir[0];
		yFlecha += arq->dir[1];

		if(xFlecha < 0){
			xFlecha = 0;
			yFlecha -= arq->dir[1];
			break;
		}

		if(xFlecha > MAXWIDTH - 1){
			xFlecha = MAXWIDTH - 1;
			yFlecha -= arq->dir[1];
			break;
		}
			
		if(yFlecha < 0){
			yFlecha = 0;
			xFlecha -= arq->dir[0];
			break;
		}

		if(yFlecha > MAXHEIGHT - 1){
			yFlecha = MAXHEIGHT - 1;
			xFlecha -= arq->dir[0];
			break;
		}

		
	}

	mapa->pos[yFlecha][xFlecha]++;
}


/* escreverTutorial()
 * Escreve na tela os comandos do jogo antes do início da partida
*/

void escreverTutorial(){

	printf("\n\nVocê (@) é um arqueiro preso num campo de grama alta.\n");
	printf("Se esgueirando pela grama alta existe algo ou alguém tentando te matar.\n");
	printf("Sua única escolha é retribuir o favor.\n\n");

	
	printf("Em seu turno, há duas opções [m]over ou [a]tirar.\n");
	printf("Após escolher sua ação, informe em qual direção será realizada. As possíveis direções são:\n\n");

	printf("nw nn ne\nww @ ee\nsw ss se\n\n");

	printf("Por fim, caso a ação escolhida seja [m]over, resta dizer a distancia desejada (valor entre 1-3).\n");

	printf("Boa sorte e que vença o melhor.\n");

}


