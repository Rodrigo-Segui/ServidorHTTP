#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
     
    // criando o socket - funcao socket
    //int socket(int domain, int type, int protocol)
    //AF_INET protocolo para comunicaçao ipv4
    //tipo de comunicacao - sock_stream - comunicacao de dois lados baseada em conexao
    // 0 procolo tcp
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
     
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
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
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1){

        printf("\n-----Aguardando por uma nova conexão-----\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                        (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        valread = read( new_socket , buffer, 1024);
        printf("%s\n",buffer );
        send(new_socket , hello , strlen(hello) , 0 );
        printf("Hello message sent\n");
        close(new_socket);
    }   
    return 0;
}