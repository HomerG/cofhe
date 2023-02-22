CC=g++
FLAGS=-O2 -std=c++17
ROOT=./seal_unx
INCS=-I${ROOT}/include
LIBS=-L${ROOT}/lib -lseal -pthread# timer.cpp
#INCS=
#LIBS=-lseal -pthread
N=13

all: compile run

%: %.cpp
	${CC} ${INCS} ${FLAGS} -c $<
	${CC} -o $@.exe $@.o ${LIBS}

compile: main

run:
	./main.exe ${N}
