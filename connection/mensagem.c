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
	
	msg->x[0] = -1;
	msg->x[1] = -1;
	msg->x[2] = -1;

	msg->y[0] = -1;
	msg->y[1] = -1;
	msg->y[2] = -1;

	msg->acao = "...";
	memset(msg->string, 0, sizeof(msg->string));
	
	return msg;
}

/* lerMensagem(Mensagem*)
 * Lê valores de uma string e os adiciona aos respectivos atributos
 * da struct Mensagem
*/

void lerMensagem(Mensagem* msg){
	for(int i = 0; i < 15; i+=7){
		msg->acao[i/7] = msg->string[i];
		msg->x[i/7] = (msg->string[i + 2] - '0') * 10 + msg->string[i + 3] - '0';
		msg->x[i/7] = msg->string[i + 1] == '1' ? msg->x[i/7] * - 1 : msg->x;

		msg->y[i/7] = (msg->string[i + 5] - '0') * 10 + msg->string[i + 6] - '0';
		msg->y[i/7] = msg->string[i + 4] == '1' ? msg->y[i/7] * - 1 : msg->y[i/7];
	}	
}


/* escreverMensagem(Mensagem*)
 * Coloca os valores da struct Mensagem no formato de string
 * preparando para o envio
*/

void escreverMensagem(Mensagem* msg){
	for(int i = 0; i < 15; i+=7){
		msg->string[i] = msg->acao[i/7];

		msg->string[i + 1] = '0';
		if(msg->x[i/7] < 0){
			msg->string[i+1] = '1';
			msg->x[i/7] *= -1;
		}

		msg->string[i+2] = (msg->x[i/7]/10) + '0';
		msg->string[i+3] = (msg->x[i/7] % 10) + '0';

		msg->string[i+4] = '0';
		if(msg->y[i/7] < 0){
			msg->string[i+4] = '1';
			msg->y[i/7] *= -1;
		}
		msg->string[i+5] = (msg->y[i/7] / 10) + '0';
		msg->string[i+6] = (msg->y[i/7] % 10) + '0';
	}
}


/* interpretarMensagem(Mensagem*, Persona*, Mapa*)
 * Analisa mensagem e reage de acordo com o que foi dito
 * deve ser utilizado APOS 'lerMensagem(Mensagem*)'
*/

void interpretarMensagem(Mensagem* msg, Persona* per, Mapa* mapa, int per_num){
	switch(msg->acao[per_num]){
		case 'm':
			// Jogador move (Servidor)
			per->x = per->x ^ msg->x[per_num];
			msg->x[per_num] = msg->x[per_num] ^ per->x;
			per->x = msg->x[per_num] ^ per->x;
	
			per->y = msg->y[per_num] ^ per->y;
			msg->y[per_num] = msg->y[per_num] ^ per->y;
			per->y = msg->y[per_num] ^ per->y;
			msg->acao[per_num] = 'e';
			break;
		case 'e':
			if(per->x != msg->x[per_num] || per->y != msg->y[per_num]){
				mapa->pos[msg->y[per_num]][msg->x[per_num]] *= -1;
				mapa->pos[msg->y[per_num]][msg->x[per_num]] -= 20; 
			}
			break;
		case 'a':
			// Jogador atira flecha (Servidor)
			registrarFlecha(per, mapa, msg);
			break;
		case 'f':
			// Oponente atira flecha (Jogadores)
			mapa->pos[msg->y[per_num]][msg->x[per_num]]++;
			break;
		case 'c':
			// Jogador coleta flecha (Servidor)
			mapa->pos[msg->y[per_num]][msg->x[per_num]]--;
			per->x = msg->x[per_num];
			per->y = msg->y[per_num];
			msg->acao[per_num] = 'r';
			break;
		case 'r':
			// Registra flecha coletada por oponente
			registrarFlechaColetada(mapa, msg->x[per_num], msg->y[per_num]);
			msg->acao[per_num] = 't';
			break;
		case 'w':
			// Vitoria (Jogadores)
			mapa->pos[msg->y[per_num]][msg->x[per_num]] *= -1;
			mapa->pos[msg->y[per_num]][msg->x[per_num]] -= 15;
			msg->acao[per_num] = 'k';
			break;
		case 'l':
			// Derrota (Jogadores)
			mapa->pos[per->y][per->x] *= -1;
			mapa->pos[per->y][per->x] -= 15;
			msg->acao[per_num] = 'o';
			break;
		case 'q':
			// Cria Archer (Jogadores)
			per->x = msg->x[per_num];
		       	per->y = msg->y[per_num];
			break;
		case 'k':
			// Kill server (Servidor)
			break;
		case 'o':
			// Game Over (Servidor) 
			msg->acao[per_num] = 'w';
			break;
	
	}
}


/* mensagemInicial(Mensagem*, Archer*, int)
 * Constroi a primeira mensagem do servidor
 * Passa informações de criação pro jogador.
*/

void mensagemInicial(Mensagem* msg, Persona* per, int clientfd){
	msg->acao = "qqq";
	msg->x[0] = per[0]->x;
	msg->y[0] = per[0]->y;
	msg->x[1] = per[1]->x;
	msg->y[1] = per[1]->y;
	msg->x[2] = per[2]->x;
	msg->y[2] = per[2]->y;
	escreverMensagem(msg);	
	if(send(clientfd, (char*)msg->string, sizeof(msg->string), 0) == -1){
		handle_error("send");
	}
}
