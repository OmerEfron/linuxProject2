# Compiler options
CC := g++
CFLAGS := -Wall -Werror -pedantic -std=c++17 -g

# Source files
SRCS := utility.cpp execution.cpp main2.cpp

# Object files
OBJS := $(SRCS:.cpp=.o)

# Executable
MAIN2 := main2

# Build target
all: $(MAIN2)

$(MAIN2): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lstdc++fs

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(MAIN2)
