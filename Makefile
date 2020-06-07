
CXX=clang++
CXXFLAGS=-std=c++2a -pedantic-errors -Wall -Weffc++ -Wextra -Wsign-conversion -Iinclude
LFLAGS=-lpthread
SRC=src
OBJ=main.o

all: options prepare main.o calc

options:
	@echo tuicalc build options:
	@echo "CXX     	= $(CXX)"
	@echo "CXXFLAGS	= $(CXXFLAGS)"

clean:
	rm --force --verbose calc result logoscreen.esc helpscreen.esc

main.o: ${SRC}/main.cpp
	${CXX} ${CXXFLAGS} -c $^

prepare:
	cp ${SRC}/logoscreen.esc ${SRC}/helpscreen.esc .

calc: ${OBJ}
	${CXX} ${CXXFLAGS} ${LFLAGS} $^ -o $@
	rm --force --verbose ${OBJ}
	@echo Now you can run the program using ./calc
