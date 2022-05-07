PROGRAM=bin/main
CC=gcc

CFLAGS=-g -Wall
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
LDLIBS =

C_FILES := $(wildcard src/*.c)
OBJS := $(patsubst src/%.c, obj/%.o, $(C_FILES))

all: $(PROGRAM)

$(PROGRAM): $(OBJS) main.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(OBJS) main.c -o $(PROGRAM)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -r bin/* obj/*