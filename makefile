# makefile para compilar o Servidor HTTP
all: printy
 
printy: main.o server.o
    gcc -o printy main.o server.o
 
main.o: main.c server.h
    gcc -o main.o main.c -c -W -Wall -ansi -pedantic
 
server.o: server.c server.h
    gcc -o server.o server.c -c -W -Wall -ansi -pedantic
 
clean:
    rm -rf *.o *~ printy