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

int main()
{

    printf("----- Servidor HTTP ------\n\n");

    // Variaveis socket servidor, vetor de socket de clientes
    int socket_server, socket_client[10];

    // Declaraçao de porta padrao, taxa maxima, tamanho cliente
    int PORT = 8080;
    int MAX_RATE = 1000;
    int LENGTH_CLIENT;

    struct sockaddr_in client;
    LENGTH_CLIENT = sizeof(client);

    // ponteiro para armazenar messagem da requisicao
    char *REQUEST_MESSAGE;
    REQUEST_MESSAGE = (char *)malloc(LENGTH_MESSAGE * sizeof(char));

    // Informaçoes do servidor
    printf("Mensagem do Servidor | Status      : Online!\n");
    printf("Mensagem do Servidor | Taxa Maxima : %d \n", MAX_RATE);
    printf("Mensagem do Servidor | Porta       : %d \n", PORT);

    // criação do socket
    //socket(int domain, int type, int protocol)
    // PARAMETROS
    // 1 - AF_INET protocolo para comunicaçao ipv4
    // 2 - tipo de comunicacao - sock_stream - comunicacao de dois lados baseada em conexao
    // 3 - protocolo tcp
    socket_server = socket(AF_INET, SOCK_STREAM, 0);

    // verificacao de erro socket
    if (socket_server < 0)
    {
        printf("\nErro! CREATE SOCKET!");
        printf("\n-----------------\n");
        exit(1);
    }

    bzero(&client, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    client.sin_addr.s_addr = INADDR_ANY;

    // Associa o socket a um endereco na rede/local - funcao bind
    // bind(int sockfd, const struct sockaddr *addr, soclen_t addrlen)
    // ARGUMENTOS
    // 1 - sockfd -socket que queremos associar um endereco a ele
    // 2 - ponteiro para uma estrutura socketaddr- info familia de protocolos, endereco na rede local  e porta
    // 3 -tamanho em bytes das estruturas passadas no segundo argumento

    if (bind(socket_server, (struct sockaddr *)&client, LENGTH_CLIENT) == -1)
    {
        printf("\nErro! Bind!");
        printf("\n-----------------\n");
        exit(1);
    }

    //permitir que o socket receba conexoes
    //maximo de conexoes simulaneas
    if (listen(socket_server, MAX_CONNECTIONS) == -1)
    {
        printf("\nErro! Listen!");
        printf("\n-----------------\n");
        exit(1);
    }

    int clients_number = 0;

    while (1)
    {
        socket_client[clients_number] = accept(socket_server, (struct sockaddr *)&client, &LENGTH_CLIENT);
        printf("Recebeu conexão cliente: %i\n", clients_number);
        FILE *fp = fdopen(socket_client[clients_number], "r+");
        printf("Mensagem do Servidor | Cliente Conectado : %d \n", socket_client[clients_number]);
        printf("---------------------- Aguardando Requisição ---------------------- \n");
        //LE REQUISICAO
        fgets(REQUEST_MESSAGE, LENGTH_MESSAGE, fp);

        // FUNCAO RESPONSAVEL POR TRATAR MENSAGEM
        treatMessage(REQUEST_MESSAGE, fp, socket_client[clients_number]);
        clients_number++;
        fclose(fp);
    }

    close(socket_server);
    return 0;
}