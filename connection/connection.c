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


/* connectPlayer(int)
 * Aceita tentativa de conexão e retorna o fd
*/ 
int conectarJogador(int servfd){
	struct sockaddr_storage client_addr;
	socklen_t sin_size;
	char s[INET6_ADDRSTRLEN];
	int playerfd;
		
	sin_size = sizeof(playerfd);
	playerfd = accept(servfd, (struct sockaddr*)&client_addr, &sin_size);
		
	inet_ntop(client_addr.ss_family, &(((struct sockaddr_in*)&client_addr)->sin_addr), s, sizeof(s));
	printf("connection from: %s\n", s);
	
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


/* client_thread()
 * Cria uma thread para executar as funções do client
*/
void* client_thread(void* ip){
}


/* server_thread()
 * Cria uma thread para executar as funções do server
*/
void* server_thread(void* serverfd){
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
