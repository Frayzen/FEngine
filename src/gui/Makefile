CC=g++
OUT= libgui.a
SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(SRCS:.cpp=.o)
CXXFLAGS = -g

all: $(OBJS)
	$(AR) rcs $(OUT) $(OBJS)

clean:
	$(RM) $(OUT) $(shell find . -name '*.o')

.PHONY: clean all

