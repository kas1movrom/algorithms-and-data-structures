all_static : main_list_st main_vector_st

main_list_st : main.o liblist.a
	gcc -o main_list main.o -L. -llist
	
main_vector_st : main.o libvec.a
	gcc -o main_vector main.o -L. -lvec
	
libvec.a : storage.o_st func.o_st stack_vector.o_st
	ar cr libvec.a storage.o func.o stack_vector.o
	
liblist.a : storage.o_st func.o_st stack_list.o_st
	ar cr liblist.a storage.o func.o stack_list.o
	
storage.o_st : storage.c
	gcc -c storage.c
	
func.o_st : func.c
	gcc -c func.c
	
stack_list.o_st : stack_list.c
	gcc -c stack_list.c
	
stack_vector.o_st : stack_vector.c
	gcc -c stack_vector.c
	

all_dynamic : main_list_d main_vec_d

main_list_d : main.o liblist.so
	gcc -o main_list main.o -L. -llist -Wl,"-rpath=."

main_vec_d : main.o libvec.so
	gcc -o main_vector main.o -L. -lvec -Wl,-rpath,.

liblist.so : storage.o_d func.o_d stack_list.o_d
	gcc -shared -o liblist.so storage.o func.o stack_list.o

libvec.so : storage.o_d func.o_d stack_vec.o_d
	gcc -shared -o libvec.so storage.o func.o stack_vector.o

storage.o_d : storage.c
	gcc -c -fPIC storage.c

func.o_d : func.c
	gcc -c -fPIC func.c

stack_list.o_d : stack_list.c
	gcc -c -fPIC stack_list.c

stack_vec.o_d : stack_vector.c
	gcc -c -fPIC stack_vector.c



main.o : main.c
	gcc -c main.c

clean_main_vector :
	rm main_vector

clean_main_list :
	rm main_list

clean_static_lib_vector :
	rm libvec.a

clean_static_lib_list :
	rm liblist.a

clean_dynamic_lib_list :
	rm liblist.so

clean_dynamic_lib_vector :
	rm libvec.so				
	
clean_o :
	rm *.o

