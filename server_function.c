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

void sendFile(char *file_path, FILE *file_pointer, int socket, int rate)
{
    FILE *new_file_pointer;
    new_file_pointer = fopen(file_path, "r+");
    if (new_file_pointer == NULL)
    {
        printf("\nError! Arquivo não encontrado !");
        printf("\n-----------------\n");
        exit(1);
    }
    else
    {
        printf("-> Arquivo encontrado \n");
        printf("-> Arquivo: %s \n", file_path);
        char *buffer = (char *)malloc(rate * sizeof(char));
        int result;
        int total = 0;
        int flag = 1;
        rewind(file_pointer);
        rewind(new_file_pointer);
        while (flag)
        {
            result = fread(buffer, 1, rate, new_file_pointer);
            sleep(1);
            if (result <= 0)
            {
                flag = 0;
            }
            if (result > 0 && flag == 1)
            {
                write(socket, buffer, result);
            }
            total = total + result;
        }
        printf("\n\n ----- Aguardando uma nova conexão ------");
        fclose(new_file_pointer);
    }
}

char *treatFileType(char *type, struct stat fileinfo)
{
    time_t rawtime;
    struct tm *timeinfo;
    time_t rawtime2;
    struct tm *timeinfo2;
    char *temp;
    char *return_info;

    if (!strcmp(type, "png\0"))
    {
        temp = "image/png";
    }
    else if (!strcmp(type, "pdf\0"))
    {
        temp = "application/pdf";
    }
    else if (!strcmp(type, "txt\0"))
    {
        temp = "text/txt";
    }
    else if (!strcmp(type, "gif\0"))
    {
        temp = "image/gif";
    }
    else if (!strcmp(type, "jpeg\0"))
    {
        temp = "image/jpeg";
    }
    else if (!strcmp(type, "jpg\0"))
    {
        temp = "image/jpeg";
    }
    else if (!strcmp(type, "html\0"))
    {
        temp = "text/html";
    }
    else if (!strcmp(type, "htm\0"))
    {
        temp = "text/html";
    }

    int size = fileinfo.st_size;
    printf("-> Tamanho: %d \n", size);
    char *size_temp = (char *)malloc(10 * sizeof(char));
    sprintf(size_temp, "%d", size);
    char *date = (char *)malloc(50 * sizeof(char));
    char *LM = (char *)malloc(50 * sizeof(char));
    char *row1;
    char *row2;
    char *row3;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(date, 50, "Date: %a, %d %b %Y %X GMT\0", timeinfo);
    printf("\n%s\n", date);
    printf("-----------------\n");

    rawtime2 = fileinfo.st_mtime;
    timeinfo2 = localtime(&rawtime2);
    strftime(LM, 50, "Last-Modified: %a, %d %b %Y %X GMT\0", timeinfo2);
    printf("\n%s\n", LM);
    printf("-----------------\n");

    row1 = "HTTP/1.0 200 OK\r\n";
    row2 = "Connection: close\r\n";
    row3 = "Server: Bizucaserver \r\n";
    return_info = (char *)malloc(500 * sizeof(char));

    sprintf(return_info, "%s%s%s%s%s%s%s%s%s%s%s%s%s", row1, row2, date, "\r\n", row3, LM, "\r\n", "Content-Length: ", size_temp, "\r\n", "Content-Type: ", temp, "\r\n\r\n");

    return return_info;
}

void treatFile(char *file_path, FILE *file_point, int socket)
{
    struct stat fileinfo;
    char *type_file;
    char *data;
    int rate = 0;

    rate = (USER_RATE * 1024 * 1024);
    data = (char *)malloc(rate * sizeof(char));

    char *new_file_path = (char *)malloc(100 * sizeof(char));

    strcpy(new_file_path, file_path);
    type_file = strtok(file_path, ".");
    type_file = strtok(NULL, "\r");
    printf("-> Tipo Arquivo: %s \n", type_file);

    data = treatFileType(type_file, fileinfo);
    printf("-> Dados: \n\n");
    printf("%s", data);
    printf("\n\n-----------------\n");
    write(socket, data, strlen(data));

    sendFile(new_file_path, file_point, socket, rate);

    printf(" \n\n");
}

void treatMessage(char *message, FILE *file_pointer, int socket)
{
    char *method;
    char *file_path;
    char *protocol;

    printf("Requisição: %s", message);
    method = strtok(message, " ");

    // TRATA REQUISICOES GET
    if (!strcmp(message, "GET\0"))
    {

        printf("-> Método: %s \n", method);
        file_path = strtok(NULL, " ");
        printf("-> Arquivo: %s \n", file_path);
        protocol = strtok(NULL, "\r");

        printf("-> Protocolo: %s \n\n", protocol);

        //VERIFICA SE PROTOCOLO HTTP 1.0
        if (!strcmp(protocol, "HTTP/1.0\0"))
        {
            printf("-> Protocolo Aceito\n");
            treatFile(file_path, file_pointer, socket);
        }
        else
        {
            printf("\nError! Protocol Não Suportado!");
            printf("\n-----------------\n");
            exit(1);
        }
    }
    else
    {
        printf("\n---------------------- %s ---------------------- \n", method);
    }
}