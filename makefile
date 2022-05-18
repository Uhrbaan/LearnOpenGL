PROGRAM=bin/main
CC=gcc

CFLAGS=-g -Wall
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
LDLIBS =

C_FILES := $(wildcard src/**/*.c)
COBJS   := $(subst ../,,$(CFILES))

all: $(PROGRAM)

$(PROGRAM): $(COBJS) main.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(COBJS) main.c -o $(PROGRAM)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -r bin/* obj/*

# makefile problem -> want to output all the .o file to bin w\ folders
# but keeping them in src/  -> how