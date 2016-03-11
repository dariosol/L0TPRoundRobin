CC=g++
OPTFLAGS = -Wall -g3 -fmessage-length=0 -std=c++11 

BINDIR       := .
INCDIMDIR    := $(DIMDIR)/dim
ODIMDIR      := $(DIMDIR)/linux

INCFLAGS = -I$(INCDIMDIR) -I/usr/local/include/dim -I.
#LIBFLAGS = $(LDFLAGS) -L$(ODIMDIR) -ldim -lpthread -lconthost \

LIBFLAGS = $(LDFLAGS) -L$(ODIMDIR) -ldim -lpthread 

all:	CustomMEPFragment.o CustomMEP.o read_primitives

#read_primitives:	read_primitives.cpp
#	g++ -g -o read_primitives.o $(INCFLAGS) $(OPTFLAGS) -c read_primitives.cpp	
#	g++ -g -DVERBOSE -o $(BINDIR)/read_primitives read_primitives.o  $(LIBFLAGS)


read_primitives: read_primitives.o CustomMEP.o CustomMEPFragment.o 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CC) -o $(BINDIR)/read_primitives read_primitives.o CustomMEP.o CustomMEPFragment.o $(LIBFLAGS) $(INCFLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

read_primitives.o: read_primitives.cpp loopethernet.h
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CC) $(OPTFLAGS) -c read_primitives.cpp $(LIBFLAGS) $(INCFLAGS) 
	@echo 'Finished building target: $@'
	@echo ' '

CustomMEP.o: CustomMEP.cpp CustomMEP.h 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CC) $(OPTFLAGS) -c CustomMEP.cpp $(LIBFLAGS) $(INCFLAGS) #./na62-farm-lib-makefile/libna62-farm-lib.a
	@echo 'Finished building target: $@'
	@echo ' '

CustomMEPFragment.o: CustomMEPFragment.cpp CustomMEPFragment.h #./na62-farm-lib-makefile/libna62-farm-lib.a
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CC) $(OPTFLAGS) -c CustomMEPFragment.cpp $(LIBFLAGS) $(INCFLAGS) #./na62-farm-lib-makefile/libna62-farm-lib.a
	@echo 'Finished building target: $@'
	@echo ' '

clean:
	rm -f $(BINDIR)/read_primitives $(BINDIR)/*.o
