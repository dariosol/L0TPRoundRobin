CC=g++
OPTFLAGS = -Wall -g3 -fmessage-length=0 -std=c++11 

BINDIR       := .
INCDIMDIR    := $(DIMDIR)/dim
ODIMDIR      := $(DIMDIR)/linux

INCFLAGS = -I$(INCDIMDIR) -I/usr/local/include/dim -I. #-I./na62-farm-lib -I./na62-farm-lib-networking


#LIBFLAGS = $(LDFLAGS) -L$(ODIMDIR) -ldim -lpthread -lconthost \

LIBFLAGS = $(LDFLAGS) -L$(ODIMDIR) -ldim -lpthread 
#	-lna62-farm-lib -L"./na62-farm-lib-makefile/" \
#	-lna62-farm-lib-networking -L"./na62-farm-lib-networking/Debug"

all:	CustomMEPFragment.o CustomMEP.o read_primitives
#all:	CustomMEPFragment.o CustomMEP.o
#all:	CustomMEPFragment.o

#read_primitives:	read_primitives.cpp
#	g++ -g -o read_primitives.o $(INCFLAGS) $(OPTFLAGS) -c read_primitives.cpp	
#	g++ -g -DVERBOSE -o $(BINDIR)/read_primitives read_primitives.o  $(LIBFLAGS)


read_primitives: read_primitives.o CustomMEP.o CustomMEPFragment.o #./na62-farm-lib-makefile/libna62-farm-lib.a #./na62-farm-lib-networking/Debug/libna62-farm-lib-networking.a
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CC) -o $(BINDIR)/read_primitives read_primitives.o CustomMEP.o CustomMEPFragment.o $(LIBFLAGS) $(INCFLAGS) ##./na62-farm-lib-makefile/libna62-farm-lib.a ./na62-farm-lib-networking/Debug/libna62-farm-lib-networking.a
	@echo 'Finished building target: $@'
	@echo ' '

read_primitives.o: read_primitives.cpp loopethernet.h #./na62-farm-lib-makefile/libna62-farm-lib.a #./na62-farm-lib-networking/Debug/libna62-farm-lib-networking.a
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CC) $(OPTFLAGS) -c read_primitives.cpp $(LIBFLAGS) $(INCFLAGS) 
	@echo 'Finished building target: $@'
	@echo ' '

CustomMEP.o: CustomMEP.cpp CustomMEP.h #./na62-farm-lib-makefile/libna62-farm-lib.a
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
