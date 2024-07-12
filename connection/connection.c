#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "connection.h"


#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); }while(0)



int connect_to_first(struct addrinfo *servinfo){
	struct addrinfo *p;
	int serverfd;

	for(p = servinfo; p != NULL; p = p->ai_next){
		serverfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(serverfd == -1){
			continue;
		}

		if(ip == NULL){
			if(bind(serverfd, p->ai_addr, p->ai_addrlen) == -1){
				close(serverfd);
				continue;
			}
		} else{
			if(connect(serverfd, p->ai_addr, p->ai_addrlen) == -1){
				close(serverfd);
				continue;
			}
		}
		break;
	}
		
	if(p == NULL){
		handle_error("addr");
	}
	return serverfd;
}


/* findConnection(char*, char*)
 * Recebe duas strings como argumento, ip e port,
 * e busca uma conexao, retorna o fd da conexao
*/
int findConnection(char* ip, char* port){
	int serverfd;
	struct addrinfo hint, *servinfo; 

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;

	if(ip == NULL){
		hint.ai_flags = AI_PASSIVE;
	}

	if(getaddrinfo(ip, port, &hint, &servinfo) != 0){
		handle_error("addrinfo");
	}

	serverfd = connect_to_first(servinfo);

	freeaddrinfo(servinfo);
	return serverfd;
}


/* connectPlayer(int, Mensagem*, Persona*[], int)
 * Aceita tentativa de conexão, retorna o fd e envia a mensagem inicial
*/ 
int conectarJogador(int servfd, Mensagem* msg, Persona* pers[], int start){
	struct sockaddr_storage client_addr;
	socklen_t sin_size;
	char s[INET6_ADDRSTRLEN];
	int playerfd;
	Persona* enviados[3];
	for(int i=0; i < 3; i++){
		enviados[i] = pers[i+start];
	}
		
	sin_size = sizeof(playerfd);
	playerfd = accept(servfd, (struct sockaddr*)&client_addr, &sin_size);
	inet_ntop(client_addr.ss_family, &(((struct sockaddr_in*)&client_addr)->sin_addr), s, sizeof(s));
	printf("connection from: %s\n", s);
	
	mensagemInicial(msg, enviados, playerfd);
	return playerfd;
}


/* run_client(lua_state)
 * Conecta com um servidor e executa duel
*/

int run_client(lua_State* L){
	char* ip = (char*)lua_tostring(L, 1);	
	pthread_t client;

	pthread_create(&client, NULL, client_thread, (void*)ip);
	pthread_join(client, NULL);
}


/* start_server(lua_State*)
 * Abre o servidor para jogadores se conectarem.
*/

int start_server(lua_State* L){
	int servfd;
	servfd = findConnection(NULL, "3490");
	
	if(listen(servfd, 10) == -1){
		handle_error("listen");
	}
	printf("Open to connect...\n");	
	lua_pushnumber(L, servfd);

	return 1;
}


void atualizar(Mensagem* msg, Mapa* map, Archer* arch, Healer* heal, Hunter* hunt, int selecionado){
	if(selecionado==0){
		atualizarArq(arch, map, msg);
	} else if(selecionado==1){
		atualizar_healer(heal, map, msg);
	} else if(selecionado==2){
		atualizar_hunter(hunter, map, msg);
	}

	atualizarMapa(mapa, arch, heal, hunt);
}


void* client_thread(void* ip){
	char* ipString = (char*)ip;
	Mapa* mapa = criarMapa();
	Mensagem* msg = criarMensagem();
	int serverfd = findConnection(ipString, "3490");

	Archer* arch = criarArq(0, 0);
	Healer* heal = novo_healer(0, 0);
	Hunter* hunt = novo_hunter(0, 0);
	Persona* bodys[3] = {
			arch->body,
			heal->body,
			hunt->body
			}

	recv(serverfd, (char*)msg->string, sizeof(msg->string), 0);
	lerMensagem(msg);
	for(int i=0;i<3;i++){
		interpretarMensagem(msg, bodys[i], mapa, i);
	}
	atualizarMapa(mapa, arch, heal, hunt);
	escreverTutorial();
	do{
		recv(serverfd, (char*)msg->string, sizeof(msg->string), 0);
		lerMensagem(msg);
		for(int i=0;i<3;i++){
			interpretarMensagem(msg, bodys[i], mapa, i);
			desenharMapa(mapa);
			if(msg->acao[i] != 'o' && msg->acao[i] != 'k'){
				atualizar(msg, mapa, arch, heal, hunt, i);
				desenharMapa(mapa);
			}
		}
		escreverMensagem(msg);
		send(serverfd, (char*)msg->string, sizeof(msg->string), 0);
	}while(msg->acao != 'k' && msg->acao != 'o');

	if(msg->acao == 'k'){
		printf("Voce ganhou!!!\n");
	}else if(msg->acao == 'o'){
		printf("Voce perdeu...\n");
	}
	apagarArq(arch);
	apagar_healer(heal);
	apagar_hunter(hunt);
	apagarMapa(mapa);
	close(serverfd);

	return 0;
}


/* server_thread()
 * Cria uma thread para executar as funções do server
*/
void* server_thread(void* serverfd){
	int* servfd = (int*)serverfd;
	int playerfd[2];
	struct addrinfo hint, *p, *servinfo;
	int turno = 0;
	Mensagem* msg = criarMensagem();
	int num_bytes;

	srand(time(NULL));
	Persona* pers[] = {
			nova_persona(rand() % MAXWIDTH, rand() % MAXHEIGHT), 
			nova_persona(rand() % MAXWIDTH, rand() % MAXHEIGHT),
			nova_persona(rand() % MAXWIDTH, rand() % MAXHEIGHT),
			nova_persona(rand() % MAXWIDTH, rand() % MAXHEIGHT),
			nova_persona(rand() % MAXWIDTH, rand() % MAXHEIGHT),
			nova_persona(rand() % MAXWIDTH, rand() % MAXHEIGHT)
			};
	Mapa* mapa = criarMapa();
	playerfd[0] = conectarJogador(*servfd, msg, pers, 0);
	playerfd[1] = conectarJogador(*servfd, msg, pers, 3);

	msg->acao = 't';
	do{
		escreverMensagem(msg);
		if(send(playerfd[turno%2], (char*)msg->string, sizeof(msg->string), 0) == -1){
			handle_error("send");
		}
		if(num_bytes = recv(playerfd[turno%2], (char*)msg->string, sizeof(msg->string), 0) == -1){
			handle_error("recv");
		}
		lerMensagem(msg);
		for(int i = 0; i < 3; i++){
			interpretarMensagem(msg, pers[i * (1 + (turno%2))], mapa, i);
			atualizarMapaServer(mapa, pers, msg);
		}
		turno++;
	} while(msg->acao != 'k');

	for(int i = 0; i < 6; i++){
		apagar_persona(pers[i]);
	}
	apagarMapa(mapa);
	close(playerfd[0]);
	close(playerfd[1]);
	close(*servfd);

	return 0;
}


/* run_server(lua_State*)
 * Executa o servidor, validações entre jogadores ocorre aqui
*/

int run_server(lua_State* L){
	int servfd = (int)lua_tonumber(L, 1);
	char* ip = (char*)lua_tostring(L, 2);

	pthread_t server, client;

	pthread_create(&server, NULL, server_thread, (void*)&servfd);
	pthread_create(&client, NULL, client_thread, (void*)ip);

	pthread_join(server, NULL);
	pthread_join(client, NULL);
}
