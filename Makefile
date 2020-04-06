# A simple Makefile for compiling small SDL projects

# set the compiler
CC := clang

# set the compiler flags
CFLAGS := -ggdb3 -O0 --std=c99 -Wall -framework SDL2 -framework SDL2_image -framework SDL2_ttf

# add header files here
HDRS := pongfunc.h

# add source files here
SRCS := pongv4.5.c pongfunc.c

# generate names of object files
OBJS := $(SRCS:.c=.o)

# name of executable
EXEC := game

# build an app bundle!
build : Makefile.build
	make -f Makefile.build

# default recipe
all: $(EXEC)

# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

# recipe for building object files $(@:.o=.c)
$(OBJS): $(SRCS) $(HDRS) Makefile
	$(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
