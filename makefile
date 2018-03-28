all: was.o

was.o: was.c
	 gcc was.c -o was.o
	 chmod 755 was.o
     
clean:
	 rm *.o
