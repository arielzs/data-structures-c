# Arquitetura

## Visão geral

Cada estrutura vive na própria pasta dentro de `src/` e é dividida em três partes:

| Parte | Papel | Exemplo |
|---|---|---|
| Header (`.h`) | Declara os tipos e a API pública da estrutura, mais a função `runXxxMenu()` | `stack.h` |
| Implementação (`.c`) | Lógica da estrutura, sem leitura de teclado | `stack.c` |
| Menu (`menu_*.c`) | Interface de terminal que usa a API da estrutura | `menu_stack.c` |

O `menu.c` (menu principal) só chama o `runXxxMenu()` de cada módulo. Por isso adicionar uma estrutura nova é: criar a pasta, expor `runXxxMenu()` no header e ligar o comando no `menu.c`.

## Build e testes

O `Makefile` descobre os arquivos sozinho (`src/*.c` e `src/*/*.c`) e adiciona cada pasta de `src/` ao include path, então um header é incluído apenas pelo nome (`#include "stack.h"`).

Para os testes, todo arquivo que **não** é `main.c` nem `menu*.c` vira a "biblioteca". Cada `tests/test_*.c` é compilado junto dela, vira um binário em `bin/tests/` e o `make test` roda todos e conta quantos passaram. Isso significa que:

- a lógica das estruturas é testada sem passar pelos menus;
- por isso as implementações **não** dependem do menu, e o menu depende delas.

## Decisões de design

**Instâncias no menu.** Cada menu guarda um vetor dinâmico de instâncias e o índice da ativa. O vetor cresce com `realloc` dobrando a capacidade. Isso permite testar operações entre duas estruturas (comparar, copiar, juntar).

**Tipos diferentes na mesma instância.** Onde uma estrutura tem variações (array dinâmico/estático, BST/AVL, hash com encadeamento/hashing duplo), o menu guarda o tipo da instância e escolhe a função certa em cada comando.

**Heap com flag `isHeap`.** O HeapSort deixa o vetor ordenado, que deixa de respeitar a propriedade de heap. A flag `isHeap` impede inserir, extrair e atualizar nesse estado. O `BuildHeap` reconstrói a heap e volta a flag para 1. Max-Heap e Min-Heap compartilham o mesmo struct `Heap`, diferenciados pelo campo `type`.

**Hashing duplo com tamanho primo.** A tabela de endereçamento aberto sempre ajusta o tamanho para o próximo primo, o que garante que a sequência de sondagem visite todos os slots. Remoções usam *tombstones* (`SLOT_DELETED`) para não quebrar a busca de chaves que colidiram.

**Rehash automático.** Ambas as tabelas dobram de tamanho quando o load factor chega a 0,75. Existem duas versões de rehash manual: uma que reaproveita os nodes/slots e outra que recria a tabela do zero.

**AVL com altura armazenada.** Cada node guarda a própria altura. A função `balance()` atualiza a altura, calcula o fator de balanceamento e aplica a rotação correta. As rotações também são expostas no menu para poderem ser testadas isoladamente.

**Pilha e fila encadeadas.** A fila guarda ponteiros para o início e o fim, então enfileirar e desenfileirar são O(1). As ordenações (insertion e merge sort) trabalham sobre os próprios nodes.

**Leitura de entrada.** `utils.c` lê inteiros com `scanf`, descarta o resto da linha e repete o pedido quando a entrada é inválida, para o menu não travar com letras. Em fim de entrada (EOF) o programa encerra.

## Convenções de código

- Nomes em inglês e `camelCase`, com prefixo da estrutura (`stackPush`, `maxHeapInsert`).
- Mensagens do programa em inglês; comentários em português.
- Funções `static` para tudo que não faz parte da API.
- Operações que podem falhar devolvem `0`/`1` ou `NULL`; as que precisam devolver um valor usam `-1` como sinal de erro (limitação conhecida, pois `-1` também é um valor válido).
- Compilação com `-Wall -Wextra -std=c11` sem warnings.
