CPLUSFLAGS = -std=c++1z
# -I /usr/include/c++/4.2.1/tr1

all: BDD.o test_BDD

BDD.h: 
BDD.cpp: 

BDD.o: BDD.cpp BDD.inc BDD.h
	clang++ $(CPLUSFLAGS) -c BDD.cpp -o BDD.o

test_BDD.o: BDD.inc BDD.h test_BDD.cpp
	clang++ $(CPLUSFLAGS) -c test_BDD.cpp -o test_BDD.o

test_BDD: BDD.o test_BDD.o
	clang++ $(CPLUSFLAGS) BDD.o test_BDD.o -o test_BDD

clean:
	rm -f BDD.o test_BDD.o test_BDD
	

