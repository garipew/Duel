#include "mensagem.h"
#include "archer.h"
#include "mapa.h"
#include "gameMaster.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>


/* criarMensagem()
 * Aloca memoria para struct Mensagem e retorna ponteiro para tal
*/

Mensagem* criarMensagem(){
	Mensagem* msg = malloc(sizeof(*msg));
	
	msg->x = -1;
	msg->y = 1;
	msg->acao = ' ';
	memset(msg->string, 0, sizeof(msg->string));
	
	return msg;
}

/* lerMensagem(Mensagem*)
 * Lê valores de uma string e os adiciona aos respectivos atributos
 * da struct Mensagem
*/

void lerMensagem(Mensagem* msg){
	msg->acao = msg->string[0];
	
	msg->x = (msg->string[2] - '0') * 10 + msg->string[3] - '0';
	msg->x = msg->string[1] == '1' ? msg->x * - 1 : msg->x;

	msg->y = (msg->string[5] - '0') * 10 + msg->string[6] - '0';
	msg->y = msg->string[4] == '1' ? msg->y * - 1 : msg->y;
}


/* escreverMensagem(Mensagem*)
 * Coloca os valores da struct Mensagem no formato de string
 * preparando para o envio
*/

void escreverMensagem(Mensagem* msg){
	msg->string[0] = msg->acao;
	
	msg->string[1] = '0';
	if(msg->x < 0){
		msg->string[1] = '1';
		msg->x *= -1;
	}

	msg->string[2] = (msg->x/10) + '0';
	msg->string[3] = (msg->x % 10) + '0';
	
	msg->string[4] = '0';
	if(msg->y < 0){
		msg->string[4] = '1';
		msg->y *= -1;
	}
	msg->string[5] = (msg->y / 10) + '0';
	msg->string[6] = (msg->y % 10) + '0';
}


/* interpretarMensagem(Mensagem*)
 * Analisa mensagem e reage de acordo com o que foi dito
 * DEVE ser utilizado APOS 'lerMensagem(Mensagem*)'
*/

void interpretarMensagem(Mensagem* msg, Archer* arq, Mapa* mapa){
	switch(msg->acao){
		case 'm':
			// Jogador move (Servidor)
			arq->x = arq->x ^ msg->x;
			msg->x = msg->x ^ arq->x;
			arq->x = msg->x ^ arq->x;
	
			arq->y = msg->y ^ arq->y;
			msg->y = msg->y ^ arq->y;
			arq->y = msg->y ^ arq->y;
			msg->acao = 'e';
			break;
		case 'e':
			if(arq->x != msg->x || arq->y != msg->y){
				mapa->pos[msg->y][msg->x] *= -1;
				mapa->pos[msg->y][msg->x] -= 20; 
			}
			break;
		case 'a':
			// Jogador atira flecha (Servidor)
			registrarFlecha(arq, mapa, msg);
			break;
		case 'f':
			// Oponente atira flecha (Jogadores)
			mapa->pos[msg->y][msg->x]++;
			break;
		case 'c':
			// Jogador coleta flecha (Servidor)
			mapa->pos[msg->y][msg->x]--;
			arq->x = msg->x;
			arq->y = msg->y;
			msg->acao = 'r';
			break;
		case 'r':
			// Registra flecha coletada por oponente
			registrarFlechaColetada(mapa, msg->x, msg->y);
			msg->acao = 't';
			break;
		case 'w':
			// Vitoria (Jogadores)
			mapa->pos[msg->y][msg->x] *= -1;
			mapa->pos[msg->y][msg->x] -= 15;
			msg->acao = 'k';
			break;
		case 'l':
			// Derrota (Jogadores)
			mapa->pos[arq->y][arq->x] *= -1;
			mapa->pos[arq->y][arq->x] -= 15;
			msg->acao = 'o';
			break;
		case 'q':
			// Cria Archer (Jogadores)
			arq->x = msg->x;
		       	arq->y = msg->y;
			break;
		case 'k':
			// Kill server (Servidor)
			break;
		case 'o':
			// Game Over (Servidor) 
			msg->acao = 'w';
			break;
	
	}
}


/* mensagemInicial(Mensagem*, Archer*, int)
 * Constroi a primeira mensagem do servidor
 * Passa informações de criação pro jogador.
*/

void mensagemInicial(Mensagem* msg, Archer* arq, int arqfd){
	msg->acao = 'q';
	msg->x = arq->x;
	msg->y = arq->y;
	escreverMensagem(msg);	
	if(send(arqfd, (char*)msg->string, sizeof(msg->string), 0) == -1){
		handle_error("send");
	}
}
