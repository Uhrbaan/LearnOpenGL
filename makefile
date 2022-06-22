PROGRAM=bin/main
CC=clang

CFLAGS= -g -Wall
# all -L libraries flags
LDFLAGS = 
# all -l libraries
LDLIBS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -lassimp

C_FILES := $(wildcard src/*.c src/**/*.c)
OBJS    := $(patsubst src/%.c, obj/%.o, $(C_FILES))

all: $(PROGRAM)

$(PROGRAM): $(OBJS) main.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(OBJS) main.c -o $(PROGRAM)

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -r -f bin/* obj/*

run:
	@./$(PROGRAM)

debug:
	@gdb $(PROGRAM)