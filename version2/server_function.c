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

int thread_count = 0; //contador do numero de threads ativas ao mesmo tempo

void sendFileimage(char *file_name, int socket, int rate, char *type){
     char *tok, *nnfile, *ponto;
      char *nfile = (char*)malloc((strlen(file_name)-1)*sizeof(char)); // aloca espaço para nfile, com tamanho -1 de filename
      int fp;
      int flg = 0;
      char pt = '.';

      strcpy(nfile,&file_name[1]); // copia conteudo de filename a partir da 2 posicao para n pegar /
      printf("%s\n",nfile );

      tok = strtok(nfile,"/");// procura por uma / no meio da string
      if(tok != NULL){ // se encontra outro / em nfile, entao existe subpasta
          puts ("Found subfolders");
          ponto = (char*)malloc((strlen(file_name)+1)*sizeof(char)); // aumenta o tamanho para 1  a mais q file_name
          ponto[0] = '\0';//inicializa ponto com \0 indicando string vazia
          nnfile = (char*)malloc((strlen(file_name))*sizeof(char));
          strncat(ponto,&pt,1);
          strcpy(nnfile,file_name); // copia conteudo de file_name
          strcat(ponto,nnfile); // coloca na frente o . para string ficar: ./pasta/arquivo.ext
          flg = 1;
          printf("ponto: %s\n", ponto);
      }

      if (!flg) // nao sei se isso aqui esta fazendo certo, parece que  ele esta entrando no if do tok sempre
        fp = open(nfile, O_RDONLY); // mas seria para escolher qual string abrir, dependendo se tem subpasta ou n
      else
        fp = open(ponto, O_RDONLY);

      if (fp > 0){ //se fp nao é 0 então achou o arquivo
          puts("Image Found.");
          int bytes;
          char buffer[LENGTH_MESSAGE];

          send(socket, "HTTP/1.0 200 OK\r\nContent-Type: image/jpeg\r\n\r\n", 45, 0);
  	      while ( (bytes=read(fp, buffer, LENGTH_MESSAGE))>0 ) // ler o arquivo no buffer enquanto nao chega no fim
  			  write (socket, buffer, bytes); // envia o conteudo no buffer para o socket
      } else { //  nao achou arquivo
          puts("Image not found in the server!");
          write(socket, "HTTP/1.0 404 Not Found\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n<!doctype html><html><body>404 File Not Found</body></html>", strlen("HTTP/1.0 404 Not Found\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n<!doctype html><html><body>404 File Not Found</body></html>"));
      }
      close(fp);
      free(nfile);
      //free(nnfile);
      //free(ponto);

}
void sendFiletexto(char *file_name, int socket, int rate, char *type)
{
       char *buffer, *nfile, *nnfile, *tok;
      int flg = 0;
      nfile = (char*)malloc((strlen(file_name)-1)*sizeof(char)); //alocando mem para nfile tamanho -1 para remover /
      FILE *fp;
      char *ponto;
      char pt = '.';

      strcpy(nfile,&file_name[1]); //copia conteudo de file_name para nfile a partir da 2 posicao, removendo / -> para arquivos na mesma pasta
      printf("%s\n",nfile );

      tok = strtok(nfile,"/"); // procura por uma outra  /  na string
      if(tok != NULL){ // se encontra outra / em nfile, entao existe subpasta
        puts ("Found subfolders");
          ponto = (char*)malloc((strlen(file_name)+1)*sizeof(char)); // aumenta o tamanho para 1  a mais q file_name
          ponto[0] = '\0'; // inicializa com \0, string vazia (necessario por malloc deixar lixo)
          nnfile = (char*)malloc((strlen(file_name))*sizeof(char));
          strncat(ponto,&pt,1); // coloca . na string ponto
          strcpy(nnfile,file_name); // copia conteudo de file_name
          strcat(ponto,nnfile); // concatena ponto . com nome do arquivo para string ficar: ./pasta/arquivo.ext
          flg = 1; // flag de subpasta
          printf("ponto: %s\n", ponto);
      }

      if (!flg)
        fp = fopen(nfile, "r");
      else
        fp = fopen(ponto, "r");

      if (fp != NULL){ // encontrou arquivo
          puts("File Found.");

          fseek(fp, 0, SEEK_END); // procura tamanho do arquivo
          long bytes_read = ftell(fp);
          fseek(fp, 0, SEEK_SET);

          send(socket, "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n", 44, 0); // Envia cabeçalho de resposta bem sucedida
          buffer = (char *)malloc(bytes_read * sizeof(char)); // aloca buffer com tamanho do arquivo

          fread(buffer, bytes_read, 1, fp); //  ler o arquivo texto no buffer
          write (socket, buffer, bytes_read); // Envia o conteudo do buffer para o socket
          free(buffer);

          fclose(fp);
      } else { // não achou arquivo
          puts("File not found in the server!");
          write(socket, "HTTP/1.0 404 Not Found\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n<!doctype html><html><body>404 File Not Found</body></html>", strlen("HTTP/1.0 404 Not Found\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n<!doctype html><html><body>404 File Not Found</body></html>"));
      }
      free(nfile);
      free(nnfile);
      free(ponto);

  
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
    puts(file_name);
    name = strtok(file_name, "."); //nome do arquivo
    extension = strtok(NULL, "."); // extensao
    if (name == NULL || extension == NULL) { 
              puts("Wrong request");
              char *message = "HTTP/1.0 400 Bad Request\r\nConnection: close\r\n\r\n<!doctype html><html><body>400 Bad Request. (You need to request to image and text files)</body></html>";
              write(sock, message, strlen(message));
    } else if (strcmp(extension, "html")== 0 ){
              sendFiletexto(file_path, sock,60, "html");
    } else if (strcmp(extension, "jpeg") == 0 ){
              sendFileimage(file_path, sock,1, "jpeg");
    } else { 
              char *message = "HTTP/1.0 400 Bad Request\r\nConnection: close\r\n\r\n<!doctype html><html><body>400 Bad Request. Not Supported File Type (Suppoerted File Types: html and jpeg)</body></html>";
              write(sock, message, strlen(message));
    }
    free(file_name);
    free(new_sock);
    shutdown(sock, SHUT_RDWR); // encerra conexao do socket
    close(sock); // destroi socket
    sock = -1;
    pthread_exit(NULL); // sai da thread
   
    
}

void treatRequest(char *message, void *new_sock)
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

void *readRequest( void *new_sock)
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
    treatRequest(message, (void *)new_sock);



}