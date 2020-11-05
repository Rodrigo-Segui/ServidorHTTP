INDEX:
	gcc -c main.c -g
	gcc -c server_function.c -g
	gcc main.o server_function.o -o exec -pthread
	./exec
