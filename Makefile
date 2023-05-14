CPP=g++
AR=ar

CFLAGS=-W -Wall -O2 -c -o
INCLUDE_PATH=-I./inc
LIB=-lz

SRC_FILE= \
	src/BobZip.cc

OBJ_FILE= \
	src/BobZip.o

LIB_FILE= libbob.a

all: $(LIB_FILE)

$(LIB_FILE) : $(OBJ_FILE)
	$(AR) crv $@ $^

%.o : %.cc
	$(CPP) $(CFLAGS) $@ $^ $(INCLUDE_PATH) $(LIB)


clear:
	rm $(OBJ_FILE) $(LIB_FILE)