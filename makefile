PROGRAM=bin/main
CC=gcc

CFLAGS=-g -Wall
LDFLAGS =
LDLIBS =

C_FILES := $(wildcard src/*.c)
OBJS := $(patsubst src/%.c, obj/%.o, $(C_FILES))

all: $(PROGRAM)

$(PROGRAM): $(OBJS) main.c
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LDLIBS) main.c -o $(PROGRAM)

obj/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -r bin/* obj/* 