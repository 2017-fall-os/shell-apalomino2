all: myShell hello boo pipetest

clean:
	rm -f *.o myShell hello pipetest boo
clear:
	rm *~ *#*
demo:
	./myShell

myShell: mytoc.o myShell.o utility.o

myShell.o: myShell.c mytoc.h utility.h

mytoc.o: mytoc.c mytoc.h

utility.o: utility.c utility.h

hello.o: hello.c

boo.o: boo.c

pipetest.o: pipetest.c utility.h
