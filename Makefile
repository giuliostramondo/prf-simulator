SRCDIR=src
OBJDIR=obj
BINDIR=bin

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(BINDIR)/prf_console:	
	bison -d src/parser.y && flex src/scanner.lex
	mv parser.tab.c src/parser.tab.c 
	mv parser.tab.h src/parser.tab.h 
	mv lex.yy.c src/lex.yy.c
	gcc -g src/parser.tab.c src/lex.yy.c src/prf.c src/utility.c src/collection.c -o bin/prf_console

$(BINDIR)/prf: $(OBJ)
	gcc -std=gnu99 \
	$(OBJ) \
	-o $(BINDIR)/prf

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	gcc \
	-g -std=gnu99 -c $< -o $@
	
documentation: doxy.conf
	doxygen doxy.conf

cleandoc:
	-rm -rf doc/*
clean:
	-rm $(BINDIR)/prf* $(OBJDIR)/*.o
	-rm -r $(BINDIR)/*.dSYM
	-rm src/lex.yy.c src/parser.tab.c src/parser.tab.h 
	-rm -rf *.dSYM

