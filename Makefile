CC=g++
CXXFLAGS= -Wall -Wextra -Werror -pedantic -std=c++20 -Wold-style-cast -g -fsanitize=address -Wno-error=old-style-cast
LDFLAGS= -lglfw -lGL -lGLEW -lm -lglm -lassimp -fsanitize=address
SRC_DIR=./src
CPPFLAGS= -I$(SRC_DIR)
SRCS = $(shell find $(SRC_DIR) \( -name '*.cc' -o -name '*.c' \) -a ! -path '*/tests/*')
OBJS = $(SRCS:.cc=.o)

engine: CFLAGS += 
engine: LDFLAGS += 
engine: all 
	$(CC) -o $@  $(OBJS) $(LDFLAGS)

debug: CFLAGS += -g
debug: LDFLAGS += -g
debug: all 
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

src/include/glad/glad.o: src/include/glad/glad.c
	$(CC) -c $< -o $@

all: $(OBJS)
	$(CC) -c $< -o $@ $(CFLAGS) -I.$(SRC_DIR)

clean:
	$(RM) $(OUT) $(shell find . -name '*.o') engine debug

.PHONY: clean all engine debug
