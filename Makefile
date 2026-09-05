CC = gcc

# Inclui a pasta include/ e todas as subpastas de src/ automaticamente,
# assim "#include \"array.h\"" funciona de qualquer arquivo do projeto,
# nao importa em que pasta ele esteja (nao precisa editar isso ao criar um novo modulo).
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