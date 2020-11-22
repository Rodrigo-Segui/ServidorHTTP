
#define MAX_CONNECTIONS 10
#define LENGTH_MESSAGE 1024
#define USER_RATE 1

sem_t mutex; // para controlar o contador de threads
sem_t mutex_rate;
struct sockaddr_in server, client;
<<<<<<< HEAD
#define PATH "/home/user/Documentos/github/ServidorHTTP/version3/arquivos"
//char ipstr[INET6_ADDRSTRLEN];
 
=======
#define PATH "/home/arthur/ServidorHTTP/version3/arquivos"

>>>>>>> 3fd302e2d29287577ae9dee13824ede41f897226
//DECLARACAO DA FUNCOES
int rateControl(char *ip);
void sendFile(char *file_name, int socket, int rate, char *type, int *ip);
void treatFileType(char *file_path, void *new_sock);
void treatFile(char *message, void *new_sock);
void *treatMessage( void *new_sock);
<<<<<<< HEAD
int* identify(int socket);
=======
void identify(int socket, char *ipstr);
>>>>>>> 3fd302e2d29287577ae9dee13824ede41f897226
