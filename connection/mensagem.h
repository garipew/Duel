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


Mensagem* nova_mensagem();
void ler_mensagem(Mensagem*);
void escrever_mensagem(Mensagem*);
void interpretar_mensagem(Mensagem*, Persona*[], Mapa*);
void mensagem_inicial(Mensagem*, Persona*[], int);
#endif
