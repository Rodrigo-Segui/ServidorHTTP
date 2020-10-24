#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
    int socket_servidor, socket_cliente, valread;
    int opt = 1;
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    
    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons( PORT )
    };

    int addrlen = sizeof(address);


     
    // criando o socket - funcao socket
    //int socket(int domain, int type, int protocol)
    //AF_INET protocolo para comunicaçao ipv4
    //tipo de comunicacao - sock_stream - comunicacao de dois lados baseada em conexao
    // 0 procolo tcp
    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
    if ((socket_servidor) == 0)
    {
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
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // permitir que o socket receba conexoes
    //maximo de conexoes simulaneas
    if (listen(socket_servidor, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1){

        printf("\n-----Aguardando por uma nova conexão-----\n");
        //funcao accept  espera por conecoes
        socket_cliente = accept(socket_servidor, (struct sockaddr *)&address,
                        (socklen_t*)&addrlen);
        if (socket_cliente < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        valread = read( socket_cliente , buffer, 1024);

        printf("%s\n",buffer );
        send(socket_cliente , hello , strlen(hello) , 0 );
        printf("Hello message sent\n");
        close(socket_cliente);
    }   
    return 0;
}