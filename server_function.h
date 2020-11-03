

#define MAX_CONNECTIONS 3
#define LENGTH_MESSAGE 1025
#define USER_RATE 1

//DECLARACAO DA FUNCOES
void sendFile(char *file_path, FILE *file_pointer, int socket, int rate);
char *treatFileType(char *type, struct stat fileinfo);
void treatFile(char *file_path, FILE *file_point, int socket);
void *connectionandtreatMessage( void *new_socket_client);

