CC=g++
CXXFLAGS= -Wall -Wextra -Werror -pedantic -std=c++20 -Wold-style-cast -g -fsanitize=address -Wno-error=old-style-cast
LDFLAGS= -Lsrc/gui/ -lgui -lglfw -lGL -lGLEW -lm -lglm -lassimp -fsanitize=address
SRC_DIR=./src
CPPFLAGS= -I$(SRC_DIR)
SRCS = $(shell find $(SRC_DIR) \( -name '*.cc' -o -name '*.c' \))
OBJS = $(addsuffix .o, $(basename $(SRCS)))

engine: CFLAGS += 
engine: LDFLAGS += 
engine: all
	$(CC) -o $@  $(OBJS) $(LDFLAGS)

source: CFLAGS += -g
source: LDFLAGS += -g
source: all 
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

all: $(OBJS)
	make -C src/gui
	$(CC) -c $< -o $@ $(CFLAGS) -I.$(SRC_DIR)

clean:
	$(RM) $(OUT) $(shell find . -name '*.o') engine debug
	make -C src/gui clean


.PHONY: clean all engine debug libs
