
struct client {
    int ip;
    int qtde;
    struct client *prox;
}; typedef struct client Client;

struct queue {
    Client *ini;
    Client *fim; 
}; typedef struct queue Queue;
Queue *f;

Queue *fila_cria();
void fila_insere_atualiza (Queue *f, int ip);
char *fila_retira(Queue *f, int ip);
void fila_libera (Queue *f);
void printar_fila(Queue *f);
int qts_clientes(Queue *f);