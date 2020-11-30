# Servidor HTTP

## VERSAO 1
## Rodar servidor
```
make
```
## Rodar Cliente
```
telnet localhost 8080
GET "caminhoarquivo/nomearquivo HTTP/1.0
```

## VERSAO 2
## Rodar servidor
```
make
```
## Rodar Cliente
```
http://localhost:8080/brasil.html
```

## VERSAO 3
## Rodar servidor
```
make
./exec PORTA MAX_NUMERO_CONEXOES_CLIENTES
```
## Rodar Cliente
```
http://localhost:PORTA/brasil.html
```



## Autores
Arthur Teixeira Jardim, Marcelo Marchioro Cordeiro e Rodrigo Acosta Segui.

## Referências

- https://github.com/ozgurhepsag/Multi-threaded-HTTP-Server
- https://github.com/eduardoveiga/ServidorHTTP
- G, Gracioli.(2011). Sockets em C, http://www-usr.inf.ufsm.br/~giovani/sockets.html, Outubro.
- Socket Programming in C, https://bitwordblog.wordpress.com/2017/01/17/socket-programming-in-c/, Outubro.
- R, Lopes, (2011),  Simples servidor http com concorrência feito em C, https://www.vivaolinux.com.br/script/Simples-servidor-http-com-concorrencia-feito-em-C/, Outubro. 
- F, Junior. (2011), Web Server em C, https://www.codigofonte.com.br/codigos/web-server-em-c, Outubro.
- H, Costa. (2012), Programação de Sockets em C/C++,https://student.dei.uc.pt/~hpcosta/rc/rcMod7BAula2.pdf, Outubro.
- FOROUZAN, Behrouz A. Redes de computadores uma abordagem top-down. 1. Porto Alegre AMGH 2013 1 recurso online ISBN 9788580551693.


“Este projeto foi desenvolvido integralmente pela equipe, sem ajuda não autorizada de alunos não membros do projeto no processo de codificação.”


