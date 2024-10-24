CC=g++
CFLAGS= -Wall -Wextra -Werror -pedantic -std=c++20 -Wold-style-cast
LDFLAGS= 
OUT= out
SRCS = $(shell find $(SRC_DIR) -name '*.cc')
OBJS = $(SRCS:.c=.o)

all: $(OBJS)
	$(CC) $(LDFLAGS) -o $(OUT) $(OBJS)

clean:
	$(RM) $(OUT) $(shell find . -name '*.o')

.PHONY: clean all

