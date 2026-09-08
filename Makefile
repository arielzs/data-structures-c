CC = gcc

# Inclui a pasta include/ e todas as subpastas de src/ automaticamente,
# assim "#include \"array.h\"" funciona de qualquer arquivo do projeto,
# nao importa em que pasta ele esteja (nao precisa editar isso ao criar um novo modulo).

INCLUDE_DIRS = $(shell find src -type d)
CFLAGS = -Wall -Wextra -std=c11 -Iinclude $(addprefix -I,$(INCLUDE_DIRS))

# ===================== Build principal =====================

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

# ===================== Testes =====================

# Todo .c de src/, menos main.c (tem seu proprio main, ia conflitar com o
# main de cada teste) e os menu*.c (nao sao necessarios pra testar a logica pura)

ALL_SRC = $(shell find src -name "*.c")
MENU_SRC = $(shell find src -name "menu*.c")
LIB_SRC = $(filter-out src/main.c $(MENU_SRC), $(ALL_SRC))

TEST_SRC = $(wildcard tests/test_*.c)
TEST_BINS = $(TEST_SRC:tests/%.c=bin/tests/%)

test: $(TEST_BINS)
	@echo ""
	@echo "===================== Rodando testes ====================="
	@passed=0; failed=0; \
	for t in $(TEST_BINS); do \
		echo ""; \
		echo "-- $$t --"; \
		if $$t; then \
			passed=$$((passed + 1)); \
		else \
			failed=$$((failed + 1)); \
			echo "FALHOU: $$t"; \
		fi; \
	done; \
	echo ""; \
	echo "============================================================"; \
	echo "$$passed passaram, $$failed falharam"; \
	if [ $$failed -gt 0 ]; then exit 1; fi

# Cada teste vira um binario proprio, compilado direto com o codigo do projeto que ele precisa

bin/tests/%: tests/%.c $(LIB_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< $(LIB_SRC) -o $@

# ===================== Limpeza =====================

clean:
	rm -rf bin

.PHONY: all run test clean