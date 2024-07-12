#ifndef MSG
#define MSG

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); }while(0)

typedef struct persona Persona;
typedef struct arch Archer;
typedef struct mapa Mapa;
typedef struct healer Healer;
typedef struct hunter Hunter;

typedef struct msg{

	int x;
	int y;
	char acao;
	char string[7];

} Mensagem;


Mensagem* criarMensagem();
void lerMensagem(Mensagem*);
void escreverMensagem(Mensagem*);
void interpretarMensagem(Mensagem*, Archer*, Mapa*);
void mensagemInicial(Mensagem*, Archer*, int);
#endif
