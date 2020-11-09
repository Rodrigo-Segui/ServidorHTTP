
#define MAX_CONNECTIONS 10
#define LENGTH_MESSAGE 1024
#define USER_RATE 1

sem_t mutex; // para controlar o contador de threads
sem_t mutex_rate;
struct sockaddr_in server, client;
#define PATH "/home/user/Documentos/github/ServidorHTTP/version2/sources"

//DECLARACAO DA FUNCOES
int rateControl();
void sendFile(char *file_name, int socket, int rate, char *type);
void treatFileType(char *file_path, void *new_sock);
void treatFile(char *message, void *new_sock);
void *treatMessage( void *new_sock);

