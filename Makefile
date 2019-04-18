shell: main.o get_path.o get_arg.o redirect.o
	gcc -g main.o get_path.o get_arg.o redirect.o -o shell

main.o: main.c
	gcc -g -c main.c

get_path.o: get_path.c
	gcc -g -c get_path.c

get_arg.o: get_arg.c
	gcc -g -c get_arg.c

redirect.o: redirect.c
	gcc -g -c redirect.c

clean:
	rm *.o shell
