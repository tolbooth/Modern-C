CC=gcc
INCDIRS=./include ../utils
CODEDIRS=. ../utils
OPT=-O0
CFLAGS=-Wall -Wextra -g $(foreach D,$(INCDIRS),-I$(D)) $(OPT)

CFILES=$(foreach D, $(CODEDIRS), $(wildcard $(D)/*.c))
OBJECTS=$(patsubst %.c,%.o,$(CFILES))

BINARY=bin

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(BINARY) $(OBJECTS)
