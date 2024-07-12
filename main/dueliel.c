#include "gameMaster.h"
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <pthread.h>


int main(int argc, char* argv[]){
	
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushcfunction(L, run_client);
	lua_setglobal(L, "run_client");

	lua_pushcfunction(L, run_server);
	lua_setglobal(L, "run_server");

	lua_pushcfunction(L, start_server);
	lua_setglobal(L, "start_server");
	
	lua_pushnumber(L, argc);
	lua_setglobal(L, "argc");

	if(argc == 2){
		lua_pushstring(L, argv[1]);
		lua_setglobal(L, "argv");
	}

	luaL_dofile(L, "duel.lua");
	lua_close(L);
	
	
	return 0;

}
