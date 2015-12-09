# ccconf example CXX=g++ CXXFLAGS=-std=c++1y -Wall -g -I. -Wsign-promo
CXX      = g++  
CXXFLAGS = -std=c++1y -Wall -g -I. -Wsign-promo  

.PHONY : all clean
all : example
clean :
	rm -f example example.o tags

tags : *.h example.cc 
	ctags *.h example.cc 

example : example.o
	${CXX} ${LDFLAGS} -o example example.o
example.o: example.cc traits_adaptors.h
