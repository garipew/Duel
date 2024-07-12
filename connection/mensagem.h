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

	int x[3];
	int y[3];
	char acao[3];
	char string[21]; // a-x-y

} Mensagem;


Mensagem* nova_mensagem();
void ler_mensagem(Mensagem*);
void escrever_mensagem(Mensagem*);
void interpretar_mensagem(Mensagem*, Persona*, Mapa*, int);
void mensagem_inicial(Mensagem*, Persona*[], int);
#endif
