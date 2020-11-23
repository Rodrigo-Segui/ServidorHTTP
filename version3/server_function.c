//VERSA0 3 ---

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>   // for threading, link with lpthread
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/time.h>
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
    //printf("%lu", client.sin_addr.s_addr);
    char str[256];
    sprintf(str, "%lu", client.sin_addr.s_addr);
    //printf("IP%s\n", str);
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
    struct timeval  tv1, tv2;
    struct timespec req = {0}; //estrutura da função nanosleep
    req.tv_sec = 0; // seta a quantidade de segundos para a função nanosleep
    strcpy(full_path, PATH); 
    strcat(full_path, file_name);

    if(strcmp(type, "html")== 0){
      fp = fopen(full_path, "r");
      if (fp != NULL) //FILE FOUND
      {
          puts("\nArquivo encontrado.");

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

    } else if(strcmp(type, "jpeg")== 0){
        int cont_bytes = 0;
          //sem_wait(&mutex_rate); // lock semaphore
        int taxa = rateControl();
        int tms;
          //sem_post(&mutex_rate); // lock semaphore
        printf("Rate: %i kbps", taxa);
        int num_y = taxa / 1024;
        int cont_rodadas = 0;
        double tempototal = 0;
        
        if ((fp=open(full_path, O_RDONLY)) > 0) // se encontro imagem
        {
            puts("\nImagem encontrada.");
            int bytes;
            char buffer[LENGTH_MESSAGE];
            send(socket, "HTTP/1.0 200 OK\r\nContent-Type: image/jpeg\r\n\r\n", 45, 0);
	        while ( (bytes=read(fp, buffer, LENGTH_MESSAGE))>0 ) { // lendo o arquivo do buffer
                cont_rodadas  = cont_rodadas + 1;

                gettimeofday(&tv1, NULL);
                write (socket, buffer, bytes); // enviando jpeg para cliente
                gettimeofday(&tv2, NULL);

                cont_bytes = cont_bytes + bytes;

                //printf ("\nTempo da rodada = %f seconds\n", (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +(double) (tv2.tv_sec - tv1.tv_sec));
                
                double tempo_de_execucao = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
                double g = 1 - tempo_de_execucao;
                tempototal = tempo_de_execucao + tempototal;

                //printf("\nTempo de espera ate 1s de cada rodada : %f \n", g);
                
                if(cont_rodadas == num_y){
                    double t = 1 - tempototal;
                    t = t * 1000; // transforma segundo em milisegundo
                    int aux = (int) t; //arredonda o valor de t para inteiro
                    req.tv_nsec = aux * 1000000L; //calcula milisegundo para nanosegundo
                    printf("\nTempo de espera total de %d rodadas ate 1s:* %i *", num_y, aux);
                    nanosleep(&req, (struct timespec *)NULL); //função nanosleep
                    cont_rodadas = 0;
                    tempototal = 0;
                }     		     
            }
            printf("\nNº de bytes trasferidos %i: ", cont_bytes);
        }
        else {// se nao encontrar arquivo entra aqui{
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
    fprintf("\nArquivo: %s", file_name);
    puts(file_name);
    name = strtok(file_name, "."); //nome do arquivo
    extension = strtok(NULL, "."); // extensao
    if (name == NULL || extension == NULL) { 
              puts("Wrong request");
              char *message = "HTTP/1.0 400 Bad Request\r\nConnection: close\r\n\r\n<!doctype html><html><body>400 Bad Request. (You need to request to image and text files)</body></html>";
              write(sock, message, strlen(message));
    } else if (strcmp(extension, "html")== 0 ){
              sem_wait(&mutex); // 
              sendFile(file_path, sock,30, "html");
              sem_post(&mutex); // release semaphore
    } else if (strcmp(extension, "jpeg") == 0 ){
              sem_wait(&mutex); // lock semaphore 

              sendFile(file_path, sock,30, "jpeg");
              sem_post(&mutex); // release semaphore
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
    printf("Metodo: %s\n", method);
    if (strncmp(method, "GET\0", 4) == 0){
        file_path = strtok(NULL, " \t"); // endereco
    }
    else if((strncmp(method, "POST\0", 5) == 0)){
      puts("Method not implemented\n");
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

    printf("-> Mensagem do Servidor | Cliente Conectado : %d \n", new_socket_client);
    printf("\n/////////// Aguardando Requisição /////////// \n\n");

    // RECEBE REQUISICOES
    request =  recv(new_socket_client, message, LENGTH_MESSAGE, 0);
    
    // TRATA REQUISICOES
    treatFile(message, (void *)new_sock);



}