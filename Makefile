# directories
DIR_ROOT=$(abspath ../..)
DIR_INTER=$(abspath .)
DIR_POLY=$(DIR_ROOT)/polymul

# build directories
DIR_BUILD=$(DIR_INTER)/build
DIR_BIN=$(DIR_BUILD)/bin

# source directories
DIR_SRC=$(DIR_INTER)/src
DIR_COFHE=$(DIR_SRC)/cofhe
DIR_COM=$(DIR_SRC)/communication
DIR_IO=$(DIR_SRC)/io
DIR_MATH=$(DIR_SRC)/math
DIR_SUP=$(DIR_SRC)/support
DIR_TEST=$(DIR_SRC)/test
DIR_TYPE=$(DIR_SRC)/type
DIR_POLYTYPE=$(DIR_POLY)/type
DIR_E3=$(DIR_ROOT)/e3elems
DIR_MAIN=$(DIR_TEST)

MPIR=0
ifeq ($(MPIR),1)
DIR_BIGUN=$(DIR_E3)/mpir1
else
DIR_BIGUN=$(DIR_E3)/mpir0
endif

# compiler
CC=g++
FLAGS=-O2 -std=c++17
DEBUG=0
ifeq ($(DEBUG),1)
FLAGS+=-g
endif
INCS=-I$(DIR_COFHE) -I$(DIR_COM) -I$(DIR_IO) -I$(DIR_MATH) -I$(DIR_SUP) \
	-I$(DIR_TYPE) -I$(DIR_POLY) -I$(DIR_POLYTYPE)
CPPS=$(DIR_COFHE)/cofhe.cpp $(DIR_COM)/spi.cpp $(DIR_COM)/termios.cpp \
	$(DIR_COM)/termios2.cpp $(DIR_COM)/termite.cpp $(DIR_COM)/uart.cpp \
	$(DIR_IO)/io.cpp $(DIR_MATH)/math.cpp $(DIR_SUP)/support.cpp \
	$(DIR_TYPE)/conversion.cpp $(DIR_POLYTYPE)/ringint.cpp
LIBS=-lgmp
DEFS=-DMERSENNE_TWISTER

E3=0
ifneq ($(E3),1)
CPPS+=$(DIR_POLYTYPE)/integer.cpp
DEFS+=-DEDUINT
else
INCS+=-I$(DIR_BIGUN)
CPPS+=$(DIR_POLYTYPE)/bigint.cpp $(DIR_BIGUN)/cgtshared.cpp
endif

# extensions
CPP=.cpp
EXE=.exe
LOG=.log

TARGET=test_ntt3

all: createDirectories compile run

%: %.cpp
	$(CC) $(INCS) $(FLAGS) -o $(DIR_BIN)/$(TARGET)$(EXE) $< $(CPPS) $(LIBS) $(DEFS)

clean:
	rm -f $(DIR_BIN)/*$(EXE)
	rm -f $(DIR_BIN)/*$(LOG)

compile: createDirectories
	$(MAKE) $(DIR_MAIN)/$(TARGET)

createDirectories:
	mkdir -p $(DIR_BUILD)
	mkdir -p $(DIR_BIN)

run:
	cd $(DIR_BIN) && ./$(TARGET)$(EXE)
