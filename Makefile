SRC = $($(wildcard src/*.c))
SRC += $(wildcard include/*.c)
LIB = /usr/local/lib
CFLAGS = -L$(LIB) -lzmq