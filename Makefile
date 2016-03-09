CC=g++
OPTFLAGS = -Wall -g3 -fmessage-length=0 -std=c++11 

BINDIR       := .
INCDIMDIR    := $(DIMDIR)/dim
ODIMDIR      := $(DIMDIR)/linux

INCFLAGS = -I$(INCDIMDIR) -I/usr/local/include/dim -I. -I./na62-farm-lib -I./na62-farm-lib-networking





#LIBFLAGS = $(LDFLAGS) -L$(ODIMDIR) -ldim -lpthread -lconthost \

LIBFLAGS = $(LDFLAGS) -L$(ODIMDIR) -ldim -lpthread \
	-lna62-farm-lib -L"./na62-farm-lib/Debug" \
	-lna62-farm-lib-networking -L"./na62-farm-lib-networking/Debug"

all:	read_primitives 

#read_primitives:	read_primitives.cpp
#	g++ -g -o read_primitives.o $(INCFLAGS) $(OPTFLAGS) -c read_primitives.cpp	
#	g++ -g -DVERBOSE -o $(BINDIR)/read_primitives read_primitives.o  $(LIBFLAGS)



#read_primitives.o: read_primitives.cpp ./na62-farm-lib/Debug/libna62-farm-lib.a
#	$(CC) $(OPTFLAGS) -c read_primitives.cpp $(INCFLAGS) $(LIBFLAGS)



#read_primitives.o: read_primitives.cpp ./na62-farm-lib/Debug/libna62-farm-lib.a ./na62-farm-lib-networking/Debug/libna62-farm-lib-networking.a
#	$(CC) $(OPTFLAGS) -c read_primitives.cpp $(LIBFLAGS) $(INCFLAGS) 

#	$(CC) $(OPTFLAGS) -c read_primitives.cpp $(LIBFLAGS) $(INCFLAGS) 

read_primitives: read_primitives.o ./na62-farm-lib/Debug/libna62-farm-lib.a ./na62-farm-lib-networking/Debug/libna62-farm-lib-networking.a
	$(CC) -o $(BINDIR)/read_primitives read_primitives.o $(LIBFLAGS) $(INCFLAGS) ./na62-farm-lib/Debug/libna62-farm-lib.a ./na62-farm-lib-networking/Debug/libna62-farm-lib-networking.a

read_primitives.o: read_primitives.cpp loopethernet.h ./na62-farm-lib/Debug/libna62-farm-lib.a ./na62-farm-lib-networking/Debug/libna62-farm-lib-networking.a
	$(CC) $(OPTFLAGS) -c read_primitives.cpp $(LIBFLAGS) $(INCFLAGS) 


#read_primitives: ./na62-farm-lib/Debug/libna62-farm-lib.a read_primitives.o
#	@echo 'Building target: $@'
#	@echo 'Invoking: GCC C++ Linker'
#	$(CC) $(OPTFLAGS) -o $(BINDIR)/read_primitives $(INCFLAGS) $(LIBFLAGS)
#	@echo 'Finished building target: $@'
#	@echo ' '

clean:
	rm -f $(BINDIR)/read_primitives $(BINDIR)/*.o
