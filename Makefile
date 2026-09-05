CC = gcc

INCLUDE_DIRS = $(shell find src -type d)
CFLAGS = -Wall -Wextra -std=c11 -Iinclude $(addprefix -I,$(INCLUDE_DIRS))

TARGET = bin/data-structures-c

SRC = $(shell find src -name "*.c")

OBJ = $(SRC:src/%.c=bin/%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(OBJ) -o $@

bin/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf bin

.PHONY: all run clean