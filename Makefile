# Compiler options
CC := g++
CFLAGS := -Wall -Werror -pedantic -std=c++17 -g 

# Source files
SRCS := utility.cpp execution.cpp main.cpp func.cpp

# Object files
OBJS := $(SRCS:.cpp=.o)

# Executable
MAIN2 := main

# Build target
all: $(MAIN2)

$(MAIN2): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lstdc++fs -lzip

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(MAIN2)
