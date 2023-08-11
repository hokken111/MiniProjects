#
# Makefile for the test programs
#

.SUFFIXES: .o .C

#
# Compiler and loader definitions
#

LD =		ld
LDFLAGS =	

CXX =		g++
CXXFLAGS =	-g -O2 -Wall

#
# general definitions
#

MAKEFILE =      Makefile

#
# list of all object and source files
#

OBJS =  db.o buf.o error.o bufMap.o testbuf.o
SRCS =	db.cpp buf.cpp error.cpp bufMap.cpp testbuf.cpp 

all:		testbuf 

testbuf:	$(OBJS) 
		$(CXX) -o $@ $(OBJS) $(LDFLAGS)

.C.o:
		$(CXX) $(CXXFLAGS) -c $<

clean:
		rm -f core \#* *.bak *~ *.o testbuf 

depend:
		makedepend -I  /usr/um/gnu/gcc/include/g++-3 -f$(MAKEFILE) \
		$(SRCS)

# DO NOT DELETE

