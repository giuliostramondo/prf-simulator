SRCDIR=src
OBJDIR=obj
BINDIR=bin

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

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
	-rm $(BINDIR)/prf $(OBJDIR)/*.o
	-rm -rf *.dSYM

