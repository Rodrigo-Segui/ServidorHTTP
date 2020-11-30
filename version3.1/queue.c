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
#include "queue.h"

Queue *fila_cria() {
    Queue *f = (Queue*) malloc(sizeof(Queue));
    f->ini = f->fim = NULL;
    return f;
}

void fila_insere_atualiza (Queue *f, int ip) {
    int flag = 0;
    
    Client *temp;
    for(temp = f->ini; temp != NULL; temp = temp->prox) {
        if(temp->ip ==  ip) {
            temp->qtde = temp->qtde + 1;
            flag = 1;
        }
    }

    if(flag == 0) {
        Client *novo = (Client*) malloc(sizeof(Client));
        novo->ip = ip;
        //strcpy(novo->ip, ip);
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

char *fila_retira(Queue *f, int ip) {
    Client *a = NULL;
    Client *p = f->ini;

    while(p != NULL && p->ip != ip) {
        a = p;
        p = p->prox;
    }
    if(p == NULL)
        return f;
    
    if(a == NULL){ //retrura elemento
        p->qtde --;
        if(p->qtde == 0){
            f = p->prox;
        }
    }
    else {
        p->qtde --;
        if(p->qtde == 0){
            f = p->prox;
            a->prox = p->prox;
        }
    }
    free(p);
    return f;
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

void printar_fila(Queue *f) {
    Client *temp;
    for(temp = f->ini; temp != NULL; temp = temp->prox) {
        printf("\nIp: %d\n", temp->ip);
        printf("Nº de conexões: %d\n", temp->qtde);
    }
}
int qts_clientes(Queue *f){
     int qtd = 0;
     Client *temp;
    for(temp = f->ini; temp != NULL; temp = temp->prox) {
        qtd= qtd +1;
    }
    return qtd;
}