# directories
DIR_COFHE=$(abspath ../..)
DIR_PROJECT=$(DIR_COFHE)/polymul
DIR_E3=$(DIR_COFHE)/e3elems
DIR_IO=$(DIR_PROJECT)/io
DIR_TYPE=$(DIR_PROJECT)/type
MPIR=0
ifeq ($(MPIR),1)
DIR_BIGUN=$(DIR_E3)/mpir1
else
DIR_BIGUN=$(DIR_E3)/mpir0
endif

# compiler, flags, incs, and libs
CC=g++
FLAGS=-O2 -std=c++17
DEBUG=0
ifeq ($(DEBUG),1)
FLAGS+=-g
endif

INCS=-I$(DIR_PROJECT) -I$(DIR_E3) -I$(DIR_IO) -I$(DIR_TYPE)
CPPS=$(DIR_TYPE)/ringint.cpp
LIBS=$(CPPS) -lgmp
DEFINES=
E3=0
ifneq ($(E3),1)
CPPS+=$(DIR_TYPE)/integer.cpp
DEFINES+=-DEDUINT
else
INCS+=-I$(DIR_BIGUN)
CPPS+=$(DIR_TYPE)/bigint.cpp $(DIR_BIGUN)/cgtshared.cpp
endif

all:

%: %.cpp
	$(CC) $(INCS) $(FLAGS) -o $@.exe $< $(LIBS) $(DEFINES)

clean:
	rm -f *.exe
