shell: main.o get_path.o list.o which.o get_arg.o
	gcc -g main.o which.o list.o get_path.o get_arg.o -o shell

main.o: main.c
	gcc -g -c main.c

get_path.o: get_path.c
	gcc -g -c get_path.c

list.o: list.c
	gcc -g -c list.c

which.o: which.c
	gcc -g -c which.c

get_arg.o: get_arg.c
	gcc -g -c get_arg.c

clean:
	rm *.o shell
