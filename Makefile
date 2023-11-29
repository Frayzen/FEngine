CC=gcc
CFLAGS= -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
LDFLAGS= -lglfw
SRCS = $(shell find $(SRC_DIR) -name '*.c' -a ! -path '*/tests/*')
OBJS = $(SRCS:.c=.o)



all: $(OBJS)

clean:
	$(RM) $(OUT) $(shell find . -name '*.o')

.PHONY: clean all
