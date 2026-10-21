# Data Structures C

*Estruturas de dados implementadas do zero em C, com menu interativo e testes*

## O projeto

Coleção de estruturas de dados clássicas escritas em C11, sem bibliotecas externas. Cada estrutura tem sua implementação isolada, um **menu interativo** para testar todas as operações pelo terminal e um arquivo de **testes** com `assert`.

Os menus trabalham com **instâncias**: é possível criar várias estruturas do mesmo tipo (por exemplo, duas pilhas ou uma BST e uma AVL), alternar qual está **ativa** e usar comandos que envolvem duas instâncias, como comparar, copiar e juntar.

## Estruturas implementadas

| Estrutura | Variações | Pasta |
|---|---|---|
| Array | Dinâmico (cresce sozinho) e estático (capacidade fixa), com 8 algoritmos de ordenação | `src/array` |
| Hash Table | Encadeamento e endereçamento aberto com hashing duplo, com rehash automático | `src/hash_table` |
| Heap | Max-Heap e Min-Heap, com BuildHeap e HeapSort | `src/heap` |
| Queue | Fila encadeada com ponteiro para o fim | `src/queue` |
| Stack | Pilha encadeada | `src/stack` |
| Tree | BST e AVL (com as 4 rotações) | `src/tree` |
| Deque | Em desenvolvimento | `src/deque` |
| Graph | Em desenvolvimento | `src/graph` |

### Algoritmos de ordenação

Bubble Sort, Selection Sort, Insertion Sort, Counting Sort (aceita negativos), Merge Sort, Quick Sort (partição de Lomuto e de Hoare) e Heap Sort. Pilhas e filas também têm ordenação própria (insertion sort e merge sort sobre os nodes).

## Como funciona o menu

```
=== DATA STRUCTURES C ===
[1] Array
[2] Deque
[3] Graph
[4] Hash Table
[5] Heap
[6] Queue
[7] Stack
[8] Tree (BST / AVL)
[0] Exit
```

Cada submenu começa pedindo para criar uma instância (comando `[1]`). Depois dela, todos os comandos agem sobre a instância **ativa**, e o comando `[3]` troca qual é a ativa.

## Como compilar e executar

Requer `gcc` e `make`.

```bash
make          # compila o projeto em bin/data-structures-c
make run      # compila e executa o menu
make test     # compila e roda todos os testes
make clean    # apaga a pasta bin
```

## Estrutura do projeto

```
data-structures-c/
├── include/          # common.h, utils.h, menu.h (compartilhados)
├── src/
│   ├── main.c
│   ├── menu.c        # menu principal
│   ├── utils.c       # leitura segura de entrada
│   ├── array/        # dynamic_array, static_array, sorting, menu_array
│   ├── hash_table/   # chained, double hashing, menu_hash_table
│   ├── heap/         # max_heap, min_heap, menu_heap
│   ├── queue/        # queue, menu_queue
│   ├── stack/        # stack, menu_stack
│   ├── tree/         # bst, avl, menu_tree
│   ├── deque/        # em desenvolvimento
│   └── graph/        # em desenvolvimento
├── tests/            # um test_*.c por estrutura
└── docs/
    ├── ARCHITECTURE.md
    └── COMPLEXITY.md
```

## Documentação

- [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md): como o código está organizado e as decisões de design.
- [`docs/COMPLEXITY.md`](docs/COMPLEXITY.md): complexidade de tempo das operações de cada estrutura.

## Licença

MIT. Veja o arquivo [LICENSE](LICENSE).
