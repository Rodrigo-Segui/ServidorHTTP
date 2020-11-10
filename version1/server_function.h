
#define MAX_CONNECTIONS 3
#define LENGTH_MESSAGE 1025
#define USER_RATE 1

#define PATH "/home/user/Documentos/github/ServidorHTTP/version1/arquivos"

//DECLARACAO DA FUNCOES
void sendFile(char *file_name, int socket, char *type);
void treatFileType(char *file_path, void *new_sock);
void treatFile(char *message, void *new_sock);
void *treatMessage( void *new_sock);

