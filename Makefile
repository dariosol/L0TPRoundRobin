CC=g++
OPTFLAGS = -Wall -g3 -fmessage-length=0 -std=c++11 

BINDIR       := .

INCFLAGS = -I/usr/local/include/dim -I.
LIBDIM = -ldim -L/usr/local/lib/dim/
LIBFLAGS = -lpthread $(LIBDIM) 

all: CustomMEPFragment.o CustomMEP.o read_primitives  receiver thread_read_primitives

#read_primitives:	read_primitives.cpp
#	g++ -g -o read_primitives.o $(INCFLAGS) $(OPTFLAGS) -c read_primitives.cpp	
#	g++ -g -DVERBOSE -o $(BINDIR)/read_primitives read_primitives.o  $(LIBFLAGS)

read_primitives: read_primitives.o CustomMEP.o CustomMEPFragment.o 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CC) -o $(BINDIR)/read_primitives read_primitives.o CustomMEP.o CustomMEPFragment.o $(LIBFLAGS) $(INCFLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

thread_read_primitives: thread_read_primitives.o
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CC) -o $(BINDIR)/thread_read_primitives thread_read_primitives.o -lboost_system -lpthread -lboost_thread
	@echo 'Finished building target: $@'
	@echo ' '

receiver: receiver.o 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CC) -o $(BINDIR)/receiver receiver.o -lboost_system -lpthread -lboost_thread $(LIBFLAGS) $(INCFLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

#%.o: %.hpp
#	@echo 'Building file: $<'
#	@echo 'Invoking: GCC C++ Compiler'
#	$(CC) $(OPTFLAGS) -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<" $(LIBFLAGS) $(INCFLAGS)
#	@echo 'Finished building: $<'
#	@echo ' '

%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) $(OPTFLAGS) -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<" $(LIBFLAGS) $(INCFLAGS)
	#$(CC) $(OPTFLAGS) -c read_primitives.cpp $(LIBFLAGS) $(INCFLAGS) 
	@echo 'Finished building: $<'
	@echo ' '

#thread_read_primitives.o: thread_read_primitives.cpp
#	@echo 'Building target: $@'
#	@echo 'Invoking: GCC C++ Linker'
#	$(CC) $(OPTFLAGS)-c thread_read_primitives.cpp
#	@echo 'Finished building target: $@'
#	@echo ' '
#read_primitives.o: read_primitives.cpp loopethernet.h
#	@echo 'Building target: $@'
#	@echo 'Invoking: GCC C++ Linker'
#	$(CC) $(OPTFLAGS) -c read_primitives.cpp $(LIBFLAGS) $(INCFLAGS) 
#	@echo 'Finished building target: $@'
#	@echo ' '
#
#CustomMEP.o: CustomMEP.cpp CustomMEP.h 
#	@echo 'Building target: $@'
#	@echo 'Invoking: GCC C++ Linker'
#	$(CC) $(OPTFLAGS) -c CustomMEP.cpp $(LIBFLAGS) $(INCFLAGS) #./na62-farm-lib-makefile/libna62-farm-lib.a
#	@echo 'Finished building target: $@'
#	@echo ' '
#
#CustomMEPFragment.o: CustomMEPFragment.cpp CustomMEPFragment.h #./na62-farm-lib-makefile/libna62-farm-lib.a
#	@echo 'Building target: $@'
#	@echo 'Invoking: GCC C++ Linker'
#	$(CC) $(OPTFLAGS) -c CustomMEPFragment.cpp $(LIBFLAGS) $(INCFLAGS) #./na62-farm-lib-makefile/libna62-farm-lib.a
#	@echo 'Finished building target: $@'
#	@echo ' '

clean:
	rm -f $(BINDIR)/read_primitives $(BINDIR)/*.o
