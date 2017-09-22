all: myShell hello

clean:
	rm -f *.o myShell hello
clear:
	rm *~ *#*
demo:
	./myShell
myShell: mytoc.o myShell.o utility.o

myShell.o: myShell.c mytoc.h utility.h

mytoc.o: mytoc.c mytoc.h

utility.o: utility.c utility.h

hello.o: hello.c
