#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h> //open
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>

#define PORT 8080
#define MAX_CONEXOES 5
#define MSG_TAM 1025
#define TAXA_USU 1

void enviaArquivo(char *arq, FILE *sp, int sock_c, int taxa, char *dados)
{
    FILE *fp;
    fp = fopen(arq, "r+");
    if (fp == NULL)
    {
        printf("\nME - Erro! Arquivo não encontrado !");
        printf("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        exit(1);
    }
    else
    {
        printf("  MT - Arquivo encontrado \n");
        printf("  MT - Arquivo: %s \n", arq);
        char *buffer = (char *)malloc(taxa * sizeof(char));
        int result;
        int total = 0;
        int flag = 1;
        //     char c;
        rewind(sp);
        rewind(fp);
        while (flag)
        {
            result = fread(buffer, 1, taxa, fp);
            sleep(1);
            //       printf("\nLido:\n%s\n\n",buffer);
            if (result <= 0)
            {
                flag = 0;
            }
            if (result > 0 && flag == 1)
            {
                write(sock_c, buffer, result);
                //         printf("\n%d\n",result);
            }
            total = total + result;
            printf("\n%d\n", result);
            //       c=fgetc(fp);
            //       printf("%s",buffer);
            //       fputc(c,sp);
        }
        printf("\n\n  MT - Lidos: %d \n", total);
        fclose(fp);
    }
}

char *trataTipoArquivo(char *tipo, struct stat fileinfo)
{
    time_t rawtime;
    struct tm *timeinfo;
    time_t rawtime2;
    struct tm *timeinfo2;
    char *temp;
    char *retorno;
    char *hora;
    char *tamanho;
    if (!strcmp(tipo, "png\0"))
    {
        temp = "image/png";
    }
    else if (!strcmp(tipo, "pdf\0"))
    {
        temp = "application/pdf";
    }
    else if (!strcmp(tipo, "txt\0"))
    {
        temp = "text/txt";
    }
    else if (!strcmp(tipo, "gif\0"))
    {
        temp = "image/gif";
    }
    else if (!strcmp(tipo, "jpeg\0"))
    {
        temp = "image/jpeg";
    }
    else if (!strcmp(tipo, "jpg\0"))
    {
        temp = "image/jpeg";
    }
    else if (!strcmp(tipo, "html\0"))
    {
        temp = "text/html";
    }
    else if (!strcmp(tipo, "htm\0"))
    {
        temp = "text/html";
    }
    int tam = fileinfo.st_size;
    printf("  MT - TAM: %d \n", tam);
    char *tamtemp = (char *)malloc(10 * sizeof(char));
    ;
    sprintf(tamtemp, "%d", tam);
    char *date = (char *)malloc(50 * sizeof(char));
    char *LM = (char *)malloc(50 * sizeof(char));
    char *linha1;
    char *linha2;
    char *linha3;
    char *linha4;
    char *linha5;
    char *linha6;
    char *linha7;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(date, 50, "Date: %a, %d %b %Y %X GMT\0", timeinfo);
    printf("\n%s\n", date);
    printf("------------------------------------------------------------------------------------\n");

    rawtime2 = fileinfo.st_mtime;
    timeinfo2 = localtime(&rawtime2);
    strftime(LM, 50, "Last-Modified: %a, %d %b %Y %X GMT\0", timeinfo2);
    printf("\n%s\n", LM);
    printf("------------------------------------------------------------------------------------\n");
    linha1 = "HTTP/1.1 200 OK\r\n";
    linha2 = "Connection: close\r\n";
    linha4 = "Server: Bizucaserver \r\n";
    retorno = (char *)malloc(500 * sizeof(char));
    sprintf(retorno, "%s%s%s%s%s%s%s%s%s%s%s%s%s", linha1, linha2, date, "\r\n", linha4, LM, "\r\n", "Content-Length: ", tamtemp, "\r\n", "Content-Type: ", temp, "\r\n\r\n");
    return retorno;
}

void trataArquivo(char *arq, FILE *sp, int sock_c)
{

    struct stat fileinfo;
    char *tipoarquivo;
    char *linha1;
    char *linha2;
    char *linha3;
    char *linha4;
    char *dados;
    int taxa = 0;
    taxa = (TAXA_USU * 1024 * 1024);
    dados = (char *)malloc(taxa * sizeof(char));
    char *arquivo = (char *)malloc(100 * sizeof(char));
    strcpy(arquivo, arq);
    //if (stat(arq, &fileinfo) == -1)
        //{
        //  printf("\nME - Erro! Criação stat() !");
        //printf("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        //exit(1);
        //}
    tipoarquivo = strtok(arq, ".");
    tipoarquivo = strtok(NULL, "\r");
    printf("  MT - Tipo Arquivo: %s \n", tipoarquivo);
    dados = trataTipoArquivo(tipoarquivo, fileinfo);
    printf("  MT - DADOS: \n\n");
    printf("%s", dados);
    printf("\n\n------------------------------------------------------------------------------------\n");
    write(sock_c, dados, strlen(dados));
    //enviaArquivo(arquivo, sp, sock_c, taxa, dados);
    printf(" \n\n");
}

void trataMensagem(char *mensagem, FILE *sp, int sock_c)
{
    char *metodo;
    char *arquivo;
    char *protocolo;
    char *tempoVivo;
    char *retorno;
    printf("MR - %s", mensagem);

    metodo = strtok(mensagem, " ");
    if (!strcmp(mensagem, "GET\0"))
    {
        printf("---------------------- %s ---------------------- \n", metodo);
        printf(" MT - Método: %s \n", metodo);
        arquivo = strtok(NULL, " ");
        printf(" MT - Arquivo: %s \n", arquivo);
        protocolo = strtok(NULL, "\r");
        printf(" MT - Protocolo: %s \n\n", protocolo);
        if (!strcmp(protocolo, "HTTP/1.1\0"))
        {
            printf("  MT - Protocolo Aceito\n", protocolo);
            trataArquivo(arquivo, sp, sock_c);
        }
        else
        {
            printf("\nME - Erro! Protocolo não Suportado!");
            printf("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
            exit(1);
        }
    }
    else
    {
        printf("\n---------------------- %s ---------------------- \n", metodo);
    }
}
int main(int argc, char const *argv[])
{
    printf("------Servidor HTTP - Redes de Computadores 2020 ------\n\n");
    int socket_servidor, socket_cliente; //
    int TAXA_MAX = 1000;
    int valread;
    int opt = 1;
    char buffer[1024] = {0};
    char *MSG_REC;
    MSG_REC = (char *)malloc(MSG_TAM * sizeof(char));

    char *hello = "Hello from server";

    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT)};

    int addrlen = sizeof(address);

    printf("MENSAGEM SERVIDOR - Status      : Servidor ON\n");
    printf("MENSAGEM SERVIDOR - Taxa Maxima : %d \n", TAXA_MAX);
    printf("MENSAGEM SERVIDOR - Porta       : %d \n", PORT);

    // criando o socket - funcao socket
    //int socket(int domain, int type, int protocol)
    //AF_INET protocolo para comunicaçao ipv4
    //tipo de comunicacao - sock_stream - comunicacao de dois lados baseada em conexao
    // 0 procolo tcp
    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
    if ((socket_servidor) == 0)
    {
        printf("\nMENSAGEM DE ERRO - Erro! Criação do socket!");
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    //Associa o socket a um endereco na rede/local - funcao bind
    //int bind(int sockfd, const struct sockaddr *addr, soclen_t addrlen)
    //sockfd -socket que queremos associar um endereco a ele
    // ponteiro para uma estrutura socketaddr- info familia de protocolos, endereco na rede local  e porta
    // tamanho em bytes das estruturas passadas no segundo argumento
    if (bind(socket_servidor, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        printf("\n MENSAGEM DE ERRO - Erro! Bind!");
        exit(EXIT_FAILURE);
    }

    // permitir que o socket receba conexoes
    //maximo de conexoes simulaneas
    if (listen(socket_servidor, MAX_CONEXOES) < 0)
    {
        printf("\n MENSAGEM DE ERRO - Erro! Listen!");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        printf("\n-----Aguardando por uma nova conexão-----\n");
        //funcao accept  espera por conecoes
        socket_cliente = accept(socket_servidor, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen);
        printf("MENSAGEM SERVIDOR - Cliente Conectado : %d \n", socket_cliente);

        if (socket_cliente < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        ////////////////////////////////////////////////////////
        FILE *fp = fdopen(socket_cliente, "r+");

        printf("---------------------- Tratamento ---------------------- \n");
        do
        {
            fgets(MSG_REC, MSG_TAM, fp);
            trataMensagem(MSG_REC, fp, socket_cliente);
        } while (strcmp(MSG_REC, "\r\n\0"));

        printf("aquii");
        printf("teste %s", MSG_REC);
        fclose(fp);
    }

    printf("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    printf("\n\n");

    //////////////////////////////////////////////////////

    //valread = nro de caracteres
    // valread = read( socket_cliente , buffer, 1024);

    /*char texto[100], palavra[100];
        char ip[10];
        char taxa[10];
        int flag = 0;
        int i;
        char ip_cliente[50];
        char str1[50];
        char temp[50];
        int contador = 0;
        strcpy(str1, strtok(buffer, ","));
 		strcpy(ip_cliente, strtok(NULL, "\n"));

        FILE *arquivo;
        arquivo = fopen("info.txt", "r");
        if(arquivo == NULL) {
            printf("Erro ao abrir o arquivo.");
            exit(1);
        } else{
            while((fgets(texto, 99, arquivo)) != NULL){
 			    strcpy(ip, strtok(texto, "\n"));
                if(strcmp(ip_cliente, ip) == 0){
                    flag = 1;
                    fgets(palavra, 99, arquivo);
                    strcpy(taxa, strtok(palavra, "\n"));
                    break;
                }
		    }
        }

        if(flag == 1) {
            // se  ip estiver no arquivo 
            printf("\nIP: %s", ip_cliente);
            printf("\nTaxa: %s kbps\n", taxa);
        }
        
        if(flag == 0) {
            printf("\nIP: %s", ip_cliente);
            printf("\nTaxa: 1000 kbps\n");
        }
        
        printf("%s\n", buffer);
        send(socket_cliente , hello , strlen(hello) , 0);
        printf("Hello message sent\n");
        close(socket_cliente);*/
    //}
    return 0;
}