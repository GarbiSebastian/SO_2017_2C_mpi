.PHONY : all clean run new

MPIROOT = /usr

CFLAGS+= -Wall -g $(INCL)
CC=gcc
MPICC=  $(MPIROOT)/bin/mpic++
INCL= -I$(MPIROOT)/include
SRCS= consola.cpp main.cpp nodo.cpp HashMap.cpp timestamp.cpp
BIN= dist_hashmap

all: dist_hashmap

$(BIN): $(SRCS)
	$(MPICC) $(CFLAGS) -o $(BIN) $(SRCS)

clean:
	rm -f $(BIN)
	rm -f generador

new: clean all

run: all
	mpiexec -np 8 ./dist_hashmap

generador: generador.cpp
	g++ -std=c++11 -o $@ generador.cpp

test-1: dist_hashmap
	cat tests/test-01.in | mpiexec -np 8 ./dist_hashmap | diff -u - tests/test-01.expected
    
test-2: dist_hashmap
	cat tests/test-02.in | mpiexec -np 8 ./dist_hashmap | diff -u - tests/test-02.expected
    
test-3: dist_hashmap
	cat tests/test-03.in | mpiexec -np 8 ./dist_hashmap | diff -u - tests/test-03.expected
    
test-4: dist_hashmap
	cat tests/test-04.in | mpiexec -np 8 ./dist_hashmap | diff -u - tests/test-04.expected
    
test-5: dist_hashmap
	cat tests/test-05.in | mpiexec -np 8 ./dist_hashmap | diff -u - tests/test-05.expected
    
test-6: dist_hashmap
	cat tests/test-06.in | mpiexec -np 8 ./dist_hashmap | diff -u - tests/test-06.expected
    
test-7: dist_hashmap
	cat tests/test-07.in | mpiexec -np 8 ./dist_hashmap | diff -u - tests/test-07.expected

test-8: dist_hashmap
	cat tests/test-08.in | mpiexec -np 8 ./dist_hashmap | diff -u - tests/test-08.expected

test-9: dist_hashmap
	cat tests/test-09.in | mpiexec -np 8 ./dist_hashmap | diff -u - tests/test-09.expected

test-10: dist_hashmap
	cat tests/test-10.in | mpiexec -np 8 ./dist_hashmap | diff -u - tests/test-10.expected

test-11: dist_hashmap
	for i in 0 1 2 3 4 5 6 7 8 9; do sed -n "$$((i * 250 + 1)),$$(((i + 1) * 250))p" corpus > corpus-"$$i"; done
	cat tests/test-11.in | mpiexec -np 8 ./dist_hashmap | diff -u - tests/test-11.expected
	rm corpus-*

test-all: test-1 test-2 test-3 test-4 test-5 test-6 test-7 test-8 test-9 test-10 test-11
	
#./generador 15 2500 7000 4 6 0 tests/archivos/tmp/;cat tests/archivos/tmp/palabras-00* | awk -f corpus.awk | sort
