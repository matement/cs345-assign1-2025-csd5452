# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# Target executable
TARGET = hy345sh

# Source files
SRCS = hy345sh.c functions.c

# Object files
OBJS = $(SRCS:.c=.o)

HDRS = hy345sh.h

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(TARGET) $(OBJS)

# Phony targets
.PHONY: all clean