prf: prf.o main.c
	gcc -std=gnu99 -g prf.o main.c -o prf

prf.o: prf.c
	gcc -std=gnu99 -g -c prf.c -o prf.o

documentation: doxy.conf
	doxygen doxy.conf

cleandoc:
	-rm -rf doc/*
clean:
	-rm prf prf.o
	-rm -rf *.dSYM

