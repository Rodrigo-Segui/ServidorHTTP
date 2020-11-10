#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "server_function.h"



void sendFile(char *file_name, int socket, char *type)
{

    
    char *buffer;
    char *full_path = (char *)malloc((strlen(PATH) + strlen(file_name)) * sizeof(char));
    FILE *fp;

    strcpy(full_path, PATH); // 
    strcat(full_path, file_name);

    if(strcmp(type, "html")== 0){
      fp = fopen(full_path, "r");
      if (fp != NULL) //FILE FOUND
      {
          puts("File Found.");

          fseek(fp, 0, SEEK_END); // tamanho do arquivo
          long bytes_read = ftell(fp);
          fseek(fp, 0, SEEK_SET);

          send(socket, "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n", 44, 0); // envia resposta ok 200
          buffer = (char *)malloc(bytes_read * sizeof(char)); 
          
          fread(buffer, bytes_read, 1, fp); // lê o buffer
          write (socket, buffer, bytes_read); //envia html para cliente
          free(buffer);
          
          fclose(fp);
      }
      else //se nao encontrar arquivo
      {
          write(socket, "HTTP/1.0 404 Not Found\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n<!doctype html><html><body>404 File Not Found</body></html>", strlen("HTTP/1.0 404 Not Found\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n<!doctype html><html><body>404 File Not Found</body></html>"));
      }  

    free(full_path);

    }else if(strcmp(type, "jpeg")== 0){
          
          if ((fp=open(full_path, O_RDONLY)) > 0) // se encontro imagem
          {
            puts("Image Found.");
            int bytes;
            char buffer[LENGTH_MESSAGE];

            send(socket, "HTTP/1.0 200 OK\r\nContent-Type: image/jpeg\r\n\r\n", 45, 0);
	          while ( (bytes=read(fp, buffer, LENGTH_MESSAGE))>0 ) // lendo o arquivo do buffer
			      write (socket, buffer, bytes); // enviando jpeg para cliente
            }
          else // se nao encontrar arquivo entra aqui
          {
          write(socket, "HTTP/1.0 404 Not Found\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n<!doctype html><html><body>404 File Not Found</body></html>", strlen("HTTP/1.0 404 Not Found\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n<!doctype html><html><body>404 File Not Found</body></html>"));
          }

    free(full_path);
    close(fp);

    }
}

void treatFileType(char *file_path, void *new_sock)
{

    
    int sock = *((int *)new_sock);
    int rate = 1000;
    char *extension;
    char *name;
    char *file_name;
    file_name = (char *)malloc(strlen(file_path) * sizeof(char));
    strcpy(file_name, file_path);
    puts("****************\n");
    puts(file_name);
    name = strtok(file_name, "."); //nome do arquivo
    extension = strtok(NULL, "."); // extensao
    if (name == NULL || extension == NULL) { 
              puts("Wrong request");
              char *message = "HTTP/1.0 400 Bad Request\r\nConnection: close\r\n\r\n<!doctype html><html><body>400 Bad Request. (You need to request to image and text files)</body></html>";
              write(sock, message, strlen(message));
    } else if (strcmp(extension, "html")== 0 ){
            sendFile(file_path, sock, "html");
    } else if (strcmp(extension, "jpeg") == 0 ){
             sendFile(file_path, sock,"jpeg");
    } else { 
              char *message = "HTTP/1.0 400 Bad Request\r\nConnection: close\r\n\r\n<!doctype html><html><body>400 Bad Request. Not Supported File Type (Suppoerted File Types: html and jpeg)</body></html>";
              write(sock, message, strlen(message));
    }          
             
    free(file_name);
    free(new_sock);
    close(sock); // destroi socket
    sock = -1;

   
    
}

void treatFile(char *message, void *new_sock)
{
     printf("ENTROU READREQUEST");
   
    char *method;
    char *file_path;

    printf("------------------------\n");
     
    printf("Requisição: %s", message);
    printf("------------------------\n");
    method = strtok(message, " \t\n"); // pega 
    printf("metodo: %s", method);
    if (strncmp(method, "GET\0", 4) == 0){
        file_path = strtok(NULL, " \t"); // endereco
    }
    else if((strncmp(method, "POST\0", 5) == 0)){
      puts("Method not implemented");
    }
    
    printf("CHAMANDO FUNCAO TRATA TIPO DO ARQUIVO");
    treatFileType(file_path, (void *)new_sock);
}

void *treatMessage( void *new_sock)
{
    // ponteiro para armazenar messagem da requisicao
    printf("ENTROU READREQUEST");
    char message[LENGTH_MESSAGE];
    int request;
    //message = (char *)malloc(LENGTH_MESSAGE * sizeof(char));

    // pegar descritor do socket
    int new_socket_client = *((int *)new_sock);

    printf("Mensagem do Servidor | Cliente Conectado : %d \n", new_socket_client);
    printf("---------------------- Aguardando Requisição ---------------------- \n");

    // RECEBE REQUISICOES
    request =  recv(new_socket_client, message, LENGTH_MESSAGE, 0);
    
    // TRATA REQUISICOES
    printf("saiu READREQUEST");
    treatFile(message, (void *)new_sock);


}