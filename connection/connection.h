#include "mensagem.h"
#include <lua.h>

int findConnection(char*, char*);
int conectarJogador(int);
int start_server(lua_State*); 
int run_client(lua_State*);
int run_server(lua_State*);
void* client_thread(void*);
void* server_thread(void*);
