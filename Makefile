CC=g++
CFLAGS= -Wall -Wextra -Werror -pedantic -std=c++20 -Wold-style-cast
CPPFLAGS= -I.
LDFLAGS= -lglfw -lGL -lGLEW -lm
SRCS = $(shell find $(SRC_DIR) \( -name '*.cc' -o -name '*.c' \) -a ! -path '*/tests/*')
OBJS = $(SRCS:.c=.o)

engine: CFLAGS += 
engine: LDFLAGS += 
engine: all 
	$(CC) -o $@  $(OBJS) $(LDFLAGS)

debug: CFLAGS += -g
debug: LDFLAGS += -g
debug: all 
	$(CC) -o $@  $(OBJS) $(LDFLAGS)

glad.o:
	$(CC) -c glad.c -o glad.o

all: $(OBJS)

clean:
	$(RM) $(OUT) $(shell find . -name '*.o') engine debug

.PHONY: clean all engine debug
