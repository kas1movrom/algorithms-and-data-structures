main : main.o func.o table.o dialog.o
	gcc main.o func.o table.o dialog.o -lreadline -o main

main.o : main.c
	gcc -c main.c

libtable.a : func.o table.o dialog.o
	ar cr libtable.a func.o table.o dialog.o

func.o : func.c func.h
	gcc -c func.c

table.o : table.c table.h
	gcc -c table.c

dialog.o : dialog.c dialog.h
	gcc -c dialog.c	


clean : 
	rm *.o
