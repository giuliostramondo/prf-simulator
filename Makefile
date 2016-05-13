prf: prf.o main.c
	gcc -std=gnu99  prf.o main.c -o prf

prf.o: prf.c
	gcc -std=gnu99 -c prf.c -o prf.o

clean:
	-rm prf prf.o
	-rm -rf *.dSYM

