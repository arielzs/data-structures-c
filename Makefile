CC = gcc

# Pega apenas diretórios dentro de include/ e src/
INCLUDE_DIRS = include $(sort $(dir $(wildcard include/*/))) $(sort $(dir $(wildcard src/*/)))
CFLAGS = -Wall -Wextra -std=c11 $(addprefix -I,$(INCLUDE_DIRS))

# ===================== Build principal =====================

TARGET = bin/data-structures-c

# Busca arquivos .c nas pastas principais e subpastas de 1 nível
SRC = $(wildcard src/*.c) $(wildcard src/*/*.c)

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

ALL_SRC = $(wildcard src/*.c) $(wildcard src/*/*.c)
MENU_SRC = $(wildcard src/menu*.c) $(wildcard src/*/menu*.c)
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

bin/tests/%: tests/%.c $(LIB_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< $(LIB_SRC) -o $@

# ===================== Limpeza =====================

clean:
	rm -rf bin

.PHONY: all run test clean