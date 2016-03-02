BINDIR       := ./
INCDIMDIR    := $(DIMDIR)/dim
ODIMDIR      := $(DIMDIR)/linux

INCFLAGS =  -I$(INCDIMDIR) -I/usr/local/include/dim -I.
LIBFLAGS = $(LDFLAGS)  -L$(ODIMDIR) -ldim -lpthread $(LIB) -L$(LIBDIR) -lconthost 
all:	read_primitives 

read_primitives:	read_primitives.cpp
	g++ -g -o read_primitives.o $(INCFLAGS) -c read_primitives.cpp	
	g++ -g -DVERBOSE -o $(BINDIR)/read_primitives read_primitives.o  $(LIBFLAGS)

clean:
	rm -f $(BINDIR)/read_primitives *.o

