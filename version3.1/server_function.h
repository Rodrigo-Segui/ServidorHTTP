
#define LENGTH_MESSAGE 125000
#define USER_RATE 1

sem_t mutex; // para controlar o contador de threads
sem_t mutex_rate, mutex_timer;
struct sockaddr_in server, client;

#define PATH "/home/user/Documentos/github/ServidorHTTP/version3.1/arquivos"

struct client {
    char ip[10];
    int qtde;
    struct client *prox;
}; typedef struct client Client;

struct queue {
    Client *ini;
    Client *fim; 
}; typedef struct queue Queue;

//DECLARACAO DA FUNCOES
int rateControl();
void sendFile(char *file_name, int socket, int rate, char *type);
void treatFileType(char *file_path, void *new_sock);
void treatFile(char *message, void *new_sock);
void *treatMessage( void *new_sock);

Queue *fila_cria();
void fila_insere_atualiza (Queue *f, char *ip);
char *fila_retira(Queue *f);
void fila_libera (Queue *f);
