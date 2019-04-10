shell: sample_shell.o get_path.o list.o which.o glob.o
	gcc -g sample_shell.o which.o list.o get_path.o glob.o -o shell

sample_shell.o: sample_shell.c
	gcc -g -c sample_shell.c

get_path.o: get_path.c
	gcc -g -c get_path.c

list.o: list.c
	gcc -g -c list.c

glob.o: glob.c
	gcc -g -c glob.c

which.o: which.c
	gcc -g -c which.c

clean:
	rm *.o shell
