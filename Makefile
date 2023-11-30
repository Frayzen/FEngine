CC=gcc
CFLAGS= -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
CPPFLAGS= -I.
LDFLAGS= -lglfw -lGL -lGLEW -lm
SRCS = $(shell find $(SRC_DIR) -name '*.c' -a ! -path '*/tests/*')
OBJS = $(SRCS:.c=.o)

engine: CFLAGS += 
engine: LDFLAGS += 
engine: all 
	$(CC) -o $@  $(OBJS) $(LDFLAGS)

debug: CFLAGS += -g
debug: LDFLAGS += -g
debug: all 
	$(CC) -o $@  $(OBJS) $(LDFLAGS)


all: $(OBJS)

clean:
	$(RM) $(OUT) $(shell find . -name '*.o') engine debug

.PHONY: clean all engine debug
