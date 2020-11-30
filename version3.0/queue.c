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

Queue *fila_cria() {
    Queue *f = (Queue*) malloc(sizeof(Queue));
    f->ini = f->fim = NULL;
    return f;
}

void fila_insere_atualiza (Queue *f, char *ip) {
    int flag = 0;
    
    Client *temp;
    for(temp = f->ini; temp != NULL; temp = temp->prox) {
        if(strcmp(temp->ip, ip) == 0) {
            temp->qtde = temp->qtde + 1;
            flag = 1;
        }
    }

    if(flag == 0) {
        Client *novo = (Client*) malloc(sizeof(Client));
        strcpy(novo->ip, ip);
        novo->qtde = 1;
        novo->prox = NULL;
        if(f->fim == NULL && f->ini == NULL) {
            f->ini = novo;
            f->fim = novo;
        }
        else {
            f->fim->prox = novo;
            f->fim = novo;
        }
    }
}

char *fila_retira(Queue *f) {
    Client *c;
    char ip[10];
    if (f->fim == NULL && f->ini == NULL) {
        printf("Fila vazia!\n");
        exit(1);
    }
    c = f->ini;
    strcpy(ip, c->ip);
    f->ini = c->prox;
    if(f->ini == NULL)
        f->fim = NULL;
    free(c);
    return ip;
}

void fila_libera (Queue *f) {
    Client *c = f->ini;
    while(c != NULL) {
        Client *t = c->prox;
        free(c);
        c = t;
    }
    free(f);
}