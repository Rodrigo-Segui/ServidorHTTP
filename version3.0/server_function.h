
#define LENGTH_MESSAGE 125000


sem_t mutex; // para controlar o contador de threads
sem_t mutex_rate, mutex_connection;
struct sockaddr_in server, client;
int num_connections;

#define PATH "/home/user/Documentos/github/ServidorHTTP/version3.1/arquivos"

//DECLARACAO DA FUNCOES
int rateControl();
void sendFile(char *file_name, int socket, int rate, char *type);
void treatFileType(char *file_path, void *new_sock);
void treatFile(char *message, void *new_sock);
void *treatMessage( void *new_sock);



