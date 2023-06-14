main : main.o libtable.a
	gcc -lreadline -o main main.o -L. -ltable

libtable.a : func.o table.o dialog.o iterator.o
	ar cr libtable.a func.o dialog.o iterator.o table.o

main.o : main.c
	gcc -c main.c

func.o : func.c func.h
	gcc -c func.c

table.o : table.c table.h
	gcc -c table.c

dialog.o : dialog.c dialog.h
	gcc -c dialog.c

iterator.o : iterator.c iterator.h
	gcc -c iterator.c

clean :
	rm *.o
