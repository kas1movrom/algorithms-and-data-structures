main : main.o libcache.a
	gcc -lreadline -o main main.o -L. -lcache

libcache.a : func.o dialog.o table.o cache.o
	ar cr libcache.a func.o dialog.o table.o cache.o

func.o : func.c func.h
	gcc -c func.c

dialog.o : dialog.c dialog.h
	gcc -c dialog.c

table.o : table.c table.h
	gcc -c table.c

cache.o : cache.c cache.h
	gcc -c cache.c

main.o : main.c
	gcc -c main.c

clean :
	rm *.o
