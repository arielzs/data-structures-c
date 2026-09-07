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

TEST_SRC = $(wildcard tests/test_*.c)
TEST_BINS = $(TEST_SRC:tests/%.c=bin/tests/%)
LIB_SRC = $(filter-out src/main.c, $(filter-out $(wildcard src/**/menu_*.c src/menu.c), $(shell find src -name "*.c")))

test: $(TEST_BINS)
	@for t in $(TEST_BINS); do \
		echo "== $$t =="; \
		$$t || exit 1; \
	done
	@echo "Todos os testes passaram!"

bin/tests/%: tests/%.c $(LIB_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< $(LIB_SRC) -o $@