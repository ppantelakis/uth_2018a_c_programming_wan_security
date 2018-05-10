all: was.o

was.o: was.c
	 gcc was.c was_functions.c -o was.o
	 chmod 755 was.o
     
clean:
	 rm *.o
