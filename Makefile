dueliel: dueliel.o gameMaster.o archer.o mapa.o mensagem.o connection.o
	gcc dueliel.o gameMaster.o archer.o mapa.o mensagem.o connection.o -o dueliel -I/usr/include/lua5.3 -llua5.3 -lpthread

gameMaster.o: archer.h mapa.h mensagem.h gameMaster.h gameMaster.c
	gcc -c gameMaster.c -I/usr/include/lua5.3 -llua5.3 -lpthread

dueliel.o: gameMaster.h archer.h mapa.h mensagem.h dueliel.c
	gcc -c dueliel.c -I/usr/include/lua5.3 -llua5.3 -lpthread

archer.o: gameMaster.h mapa.h mensagem.h archer.h archer.c
	gcc -c archer.c -I/usr/include/lua5.3 -llua5.3

mapa.o: gameMaster.h archer.h mensagem.h mapa.h mapa.c
	gcc -c mapa.c -I/usr/include/lua5.3 -llua5.3

mensagem.o: gameMaster.h archer.h mapa.h mensagem.h mensagem.c
	gcc -c mensagem.c -I/usr/include/lua5.3 -llua5.3

connection.o: mensagem.h connection.h connection.c 
	gcc -c connection.c -I/usr/include/lua5.3 -llua5.3

clean:
	rm -rf *.o dueliel
