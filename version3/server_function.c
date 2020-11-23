//VERSA0 2 ---
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/stat.h>
#include <pthread.h>   // for threading, link with lpthread
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "server_function.h"

int rateControl(){
  char text[100], word[100];
  char ip[10];
  char taxa[10];
  int flag = 0;
  int i;
  int rate = 1000;
  char ip_cliente[50];
  char str1[50];
  char temp[50];
  int contador = 0;
  printf("%lu", client.sin_addr.s_addr);
  char str[256];
  sprintf(str, "%lu", client.sin_addr.s_addr);
  printf("%s\n", str);
 	strcpy(ip_cliente, str);

        FILE *file;
        file = fopen("info.txt", "r");
        if(file == NULL) {
            printf("Erro ao abrir o arquivo.");
            exit(1);
        } else{
            while((fgets(text, 99, file)) != NULL){
 			    strcpy(ip, strtok(text, "\n"));
                if(strcmp(ip_cliente, ip) == 0){
                    flag = 1;
                    fgets(word, 99, file);
                    strcpy(taxa, strtok(word, "\n"));
                    break;
                }
		    }
        }

        if(flag == 1) {
            // se  ip estiver no arquivo 
            printf("\nIP: %s", ip_cliente);
            printf("\nTaxa: %s kbps\n", taxa);

            rate = atoi(strtok(taxa, "\0"));
        }
        
        if(flag == 0) {
            printf("\nIP: %s", ip_cliente);
            printf("\nTaxa: 1000 kbps\n");

        }
        return rate;
        }
void sendFile(char *file_name, int socket, int rate, char *type)
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
          
          //sem_wait(&mutex_rate); // lock semaphore
          int y = rateControl();
          //sem_post(&mutex_rate); // lock semaphore
          printf("***********\n");
          printf("%i    // RATE == ", y);
          printf("***********\n");
          sleep(y);
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
             // sem_wait(&mutex); // 
              sendFile(file_path, sock,30, "html");
             // sem_post(&mutex); // release semaphore
    } else if (strcmp(extension, "jpeg") == 0 ){
              //sem_wait(&mutex); // lock semaphore 

              sendFile(file_path, sock,30, "jpeg");
              //sem_post(&mutex); // release semaphore
    } else { 
              char *message = "HTTP/1.0 400 Bad Request\r\nConnection: close\r\n\r\n<!doctype html><html><body>400 Bad Request. Not Supported File Type (Suppoerted File Types: html and jpeg)</body></html>";
              write(sock, message, strlen(message));
    }


             
            
         
             
             
    free(file_name);
    //free(new_sock);
    //shutdown(sock, SHUT_RDWR); // encerra conexao do socket
    //close(sock); // destroi socket
    //sock = -1;
    pthread_exit(NULL); // sai da thread
   
    
}

void treatFile(char *message, void *new_sock)
{
   
    char *method;
    char *file_path;

    printf("------------------------\n");
     
    printf("Requisição: %s \n", message);
    printf("------------------------\n");
    method = strtok(message, " \t\n"); // pega 
    printf("metodo: %s", method);
    if (strncmp(method, "GET\0", 4) == 0){
        file_path = strtok(NULL, " \t"); // endereco
    }
    else if((strncmp(method, "POST\0", 5) == 0)){
      puts("Method not implemented");
    }
    

    treatFileType(file_path, (void *)new_sock);
 
}

void *treatMessage( void *new_sock)
{
    // ponteiro para armazenar messagem da requisicao
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
    treatFile(message, (void *)new_sock);



}