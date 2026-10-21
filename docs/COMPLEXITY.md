# Complexidade

Complexidade de tempo das operações, considerando as implementações deste projeto. `n` é o número de elementos, `h` a altura da árvore e `k` o tamanho do intervalo de valores (no Counting Sort).

## Array

| Operação | Dinâmico | Estático |
|---|---|---|
| Acesso (`Get` / `Set`) | O(1) | O(1) |
| Inserir no fim (`Push`) | O(1) amortizado | O(1) |
| Inserir / remover em posição | O(n) | O(n) |
| Buscar valor (`Contains`) | O(n) | O(n) |
| Contar ocorrências | O(n) | O(n) |
| Remover ocorrências | O(n²) | O(n²) |
| Remover duplicatas | O(n²) | O(n²) |
| Inverter | O(n) | O(n) |
| Copiar / comparar | O(n) | O(n) |

O dinâmico dobra a capacidade com `realloc` quando enche, o que dá o custo amortizado. O estático tem capacidade fixa de 100 e recusa inserções quando cheio.

## Ordenação

| Algoritmo | Melhor | Médio | Pior | Memória extra | Estável |
|---|---|---|---|---|---|
| Bubble Sort | O(n) | O(n²) | O(n²) | O(1) | Sim |
| Selection Sort | O(n²) | O(n²) | O(n²) | O(1) | Não |
| Insertion Sort | O(n) | O(n²) | O(n²) | O(1) | Sim |
| Counting Sort | O(n + k) | O(n + k) | O(n + k) | O(n + k) | Sim |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) | Sim |
| Quick Sort (Lomuto) | O(n log n) | O(n log n) | O(n²) | O(log n) | Não |
| Quick Sort (Hoare) | O(n log n) | O(n log n) | O(n²) | O(log n) | Não |
| Heap Sort | O(n log n) | O(n log n) | O(n log n) | O(1) | Não |

O Bubble Sort tem o melhor caso O(n) porque para quando uma passada não faz trocas. No Counting Sort, `k = max - min + 1`; um intervalo muito grande consome muita memória. O Quick Sort de Hoare usa o elemento do meio como pivô.

## Pilha (lista encadeada)

| Operação | Custo |
|---|---|
| `Push` / `Pop` / `IsEmpty` | O(1) |
| `SwapTop` | O(1) |
| `MoveTopDown(k)` | O(k) |
| Buscar / contar ocorrências | O(n) |
| Remover ocorrências | O(n) |
| Inverter / copiar / comparar | O(n) |
| `Rotate` / `RotateK` | O(n) |
| `RemoveMiddle` | O(n) |
| `VerifyBalance` (parênteses) | O(n) |
| `MergeTop` / `MergeBottom` | O(n + m) |
| `Sort` (insertion com pilha auxiliar) | O(n²) |
| `MergeSort` | O(n log n) |

## Fila (lista encadeada com ponteiro para o fim)

| Operação | Custo |
|---|---|
| `Enqueue` / `Dequeue` / `Peek` | O(1) |
| `Rotate` | O(1) |
| `RotateK` | O(k), com k reduzido por módulo ao tamanho |
| `MergeMove` | O(1) |
| `MergeCopy` / `Interleave` | O(n + m) |
| Buscar / contar / remover ocorrências | O(n) |
| `FindMiddle` | O(n) |
| `Reverse` / `Copy` / `Compare` / `IsSorted` | O(n) |
| `InsertSorted` | O(n) |
| `RemoveDuplicates` | O(n²) |
| `Sort` (insertion) | O(n²) |
| `MergeSort` | O(n log n) |

## Heap (Max e Min)

| Operação | Custo |
|---|---|
| Inserir | O(log n) amortizado (pode redimensionar) |
| Extrair o topo | O(log n) |
| Atualizar elemento | O(log n) |
| `BuildHeap` | O(n) |
| `HeapSort` | O(n log n) |
| `Resize` | O(n) |

## Árvores

| Operação | BST (médio) | BST (pior) | AVL |
|---|---|---|---|
| Inserir | O(log n) | O(n) | O(log n) |
| Remover | O(log n) | O(n) | O(log n) |
| Buscar / nível de um node | O(log n) | O(n) | O(log n) |
| Percursos (em ordem, pré, pós) | O(n) | O(n) | O(n) |
| Altura / nodes / folhas | O(n) | O(n) | O(1) para a altura da raiz, O(n) para contar |
| Copiar / comparar / inverter | O(n) | O(n) | O(n) |
| Rotação simples | - | - | O(1) |
| Verificar se é BST | O(n) | O(n) | O(n) |

O pior caso da BST acontece com inserções em ordem (a árvore vira uma lista, com `h = n`). A AVL mantém `h = O(log n)` por causa das rotações.

## Hash Table

| Operação | Encadeamento | Hashing duplo |
|---|---|---|
| Inserir | O(1) médio, O(n) no pior | O(1) médio, O(n) no pior |
| Buscar | O(1) médio, O(n) no pior | O(1) médio, O(n) no pior |
| Remover | O(1) médio, O(n) no pior | O(1) médio, O(n) no pior |
| Rehash | O(n + m) | O(n + m) |
| Limpar elementos | O(m + n) | O(m) |

Aqui `m` é o tamanho da tabela. Com o load factor mantido abaixo de 0,75 pelo rehash automático, o caso médio é O(1). A tabela de encadeamento aceita chaves repetidas; a de hashing duplo recusa.
