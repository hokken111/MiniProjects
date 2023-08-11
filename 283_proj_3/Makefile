FLAGS=

all: P3Search

P3Search: main.o comlineparse.o commandexec.o myheap.o complete_binary_tree.h
	g++ $(FLAGS) -o P3Search main.cpp comlineparse.cpp commandexec.cpp myheap.cpp
	
main.o: main.cpp
	g++ $(FLAGS) -c main.cpp
	
comlineparse.o: comlineparse.cpp comlineparse.h
	g++ $(FLAGS) -c comlineparse.cpp
	
commandexec.o: commandexec.cpp commandexec.h
	g++ $(FLAGS) -c commandexec.cpp
	
myheap.o: myheap.cpp myheap.h
	g++ $(FLAGS) -c myheap.cpp
	
clean:
	rm P3Search *.o
