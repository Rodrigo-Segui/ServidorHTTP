
#define MAX_CONNECTIONS 10
#define LENGTH_MESSAGE 1024
#define USER_RATE 1

sem_t mutex; // para controlar o contador de threads
sem_t mutex_rate;
struct sockaddr_in server, client;
#define PATH "/home/user/Documentos/github/ServidorHTTP/version3/arquivos"
//char ipstr[INET6_ADDRSTRLEN];
 
//DECLARACAO DA FUNCOES
int rateControl(char *ip);
void sendFile(char *file_name, int socket, int rate, char *type, int *ip);
void treatFileType(char *file_path, void *new_sock);
void treatFile(char *message, void *new_sock);
void *treatMessage( void *new_sock);
int* identify(int socket);
