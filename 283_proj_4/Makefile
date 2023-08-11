FLAGS= -O3

all: P4Server

P4Server: main.o mybitvec.o mymap.o
	g++ $(FLAGS) -o P4Server main.cpp mybitvec.cpp mymap.cpp
	
main.o: main.cpp
	g++ $(FLAGS) -c main.cpp
	
mybitvec.o: mybitvec.cpp mybitvec.h
	g++ $(FLAGS) -c mybitvec.cpp

mymap.o: mymap.cpp mymap.h
	g++ $(FLAGS) -c mymap.cpp
	
clean: 
	rm P4Server *.o
