CC=gcc
CFLAGS= -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
LDFLAGS= -lglfw -lGL -lGLEW
SRCS = $(shell find $(SRC_DIR) -name '*.c' -a ! -path '*/tests/*')
OBJS = $(SRCS:.c=.o)

engine: CFLAGS += 
engine: LDFLAGS += 
engine: all 
	$(CC) -o $@ $(LDFLAGS) $(OBJS) 

debug: CFLAGS += -g -fsanitize=address
debug: LDFLAGS += -g -fsanitize=address
debug: all 
	$(CC) -o $@ $(LDFLAGS) $(OBJS) 


all: $(OBJS)

clean:
	$(RM) $(OUT) $(shell find . -name '*.o') engine debug

.PHONY: clean all engine debug
