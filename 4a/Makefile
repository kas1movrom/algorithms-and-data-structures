main : main.o libtree.a
	gcc -lreadline -o main main.o -L. -ltree

time : libtree.a timer.o
	gcc -lreadline -o timer timer.o -L. -ltree

libtree.a : func.o dialog.o tree.o
	ar cr libtree.a func.o dialog.o tree.o

func.o : func.c func.h
	gcc -c func.c

dialog.o : dialog.c dialog.h
	gcc -c dialog.c

tree.o : tree.c tree.h
	gcc -c tree.c

main.o : main.c
	gcc -c main.c

timer.o : timer.c
	gcc -c timer.c

clean : 
	rm *.o
