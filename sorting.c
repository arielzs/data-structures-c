#include <stdio.h>
#include <stdlib.h>
#define SIZE 5

void init_array(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("valor %d: ", i + 1);
        scanf("%d", &array[i]);
    }

    printf("\n");
}

void print_array(int *array, int size)
{
    printf("\n");

    for (int i = 0; i < size; i++)
    {
        printf("[%d] -> ", array[i]);
    }

    printf("Fim\n");
}

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

void bubble_sort(int *array, int size)
{
    int swapped;

    for (int i = 0; i < size - 1; i++)
    {
        swapped = 0;

        for (int j = 0; j < size - 1 - i; j++)
        {
            if (array[j + 1] < array[j])
            {
                swap(&array[j + 1], &array[j]);
                swapped = 1;
            }
        }

        if (!swapped)
        {
            break;
        }
    }
}

void selection_sort(int *array, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        int min_pos = i;

        for (int j = i + 1; j < size; j++)
        {
            if (array[j] < array[min_pos])
            {
                min_pos = j;
            }
        }

        if (min_pos != i)
        {
            swap(&array[i], &array[min_pos]);
        }
    }
}

void insertion_sort(int *array, int size)
{
    for (int i = 1; i < size; i++)
    {
        int key = array[i];
        int j = i - 1;

        while (j >= 0 && array[j] > key)
        {
            array[j + 1] = array[j];
            j--;
        }

        array[j + 1] = key;
    }
}

void counting_sort(int *array, int size)
{
    int max = array[0];

    for (int i = 1; i < size; i++)
    {
        if (array[i] > max)
        {
            max = array[i];
        }
    }

    int *count = calloc(max + 1, sizeof(int));
    int *output = malloc(size * sizeof(int));

    for (int i = 0; i < size; i++)
    {
        count[array[i]]++;
    }

    for (int i = 1; i <= max; i++)
    {
        count[i] += count[i - 1];
    }

    for (int i = size - 1; i >= 0; i--)
    {
        output[count[array[i]] - 1] = array[i];
        count[array[i]]--;
    }

    for (int i = 0; i < size; i++)
    {
        array[i] = output[i];
    }

    free(count);
    free(output);
}

int partitionLomute(int *array, int start, int end)
{
    int pivot = array[end];

    int i = start - 1;

    for (int j = start; j < end; j++)
    {
        if (array[j] <= pivot)
        {
            i++;
            swap(&array[i], &array[j]);
        }
    }

    swap(&array[i + 1], &array[end]);

    return i + 1;
}

void quick_sort_Lomute(int *array, int start, int end)
{
    if (start >= end)
    {
        return;
    }

    int pivotidx = partitionLomute(array, start, end);

    quick_sort_Lomute(array, start, pivotidx - 1);
    quick_sort_Lomute(array, pivotidx + 1, end);
}

int partitionHoare(int *array, int start, int end)
{
    int pivot = array[start + (end - start) / 2];

    int i = start;
    int j = end;

    while (1)
    {
        while (array[i] < pivot)
        {
            i++;
        };

        while (array[j] > pivot)
        {
            j--;
        }

        if (i >= j)
        {
            return j;
        }

        swap(&array[i], &array[j]);

        i++;
        j--;
    }
}

void quick_sort_Hoare(int *array, int start, int end)
{
    if (start >= end)
    {
        return;
    }

    int pivotidx = partitionHoare(array, start, end);

    quick_sort_Hoare(array, start, pivotidx);
    quick_sort_Hoare(array, pivotidx + 1, end);
}

void merge(int *array, int start, int mid, int end)
{
    int left_size = mid - start + 1;
    int right_size = end - mid;

    int *temp_left = malloc(left_size * sizeof(int));
    int *temp_right = malloc(right_size * sizeof(int));

    for (int i = 0; i < left_size; i++)
    {
        temp_left[i] = array[start + i];
    }

    for (int i = 0; i < right_size; i++)
    {
        temp_right[i] = array[mid + 1 + i];
    }

    int left_idx = 0;
    int right_idx = 0;
    int merged_idx = start;

    while (left_idx < left_size && right_idx < right_size)
    {
        if (temp_left[left_idx] <= temp_right[right_idx])
        {
            array[merged_idx] = temp_left[left_idx];
            left_idx++;
        }
        else
        {
            array[merged_idx] = temp_right[right_idx];
            right_idx++;
        }

        merged_idx++;
    }

    while (left_idx < left_size)
    {
        array[merged_idx] = temp_left[left_idx];
        left_idx++;
        merged_idx++;
    }

    while (right_idx < right_size)
    {
        array[merged_idx] = temp_right[right_idx];
        right_idx++;
        merged_idx++;
    }

    free(temp_left);
    free(temp_right);
}

void merge_sort(int *array, int start, int end)
{
    if (start >= end)
    {
        return;
    }

    int mid = start + (end - start) / 2;

    merge_sort(array, start, mid);
    merge_sort(array, mid + 1, end);

    merge(array, start, mid, end);
}

void heapify_down(int *array, int size, int index)
{
    while (1)
    {
        int max = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < size && array[left] > array[max])
        {
            max = left;
        }

        if (right < size && array[right] > array[max])
        {
            max = right;
        }

        if (array[index] >= array[max])
        {
            break;
        }

        swap(&array[index], &array[max]);

        index = max;
    }
}

void build_max_heap(int *array, int size)
{
    int lastParent = (size / 2) - 1;

    for (int i = lastParent; i >= 0; i--)
    {
        heapify_down(array, size, i);
    }
}

void heap_sort(int *array, int size)
{
    build_max_heap(array, size);

    while (size > 1)
    {
        swap(&array[0], &array[size - 1]);

        size--;

        heapify_down(array, size, 0);
    }
}

int main()
{
    int command;

    int array[SIZE] = {0};

    while (1)
    {
        printf("\n======= MENU =======\n");
        printf("[0] Encerra o programa\n");
        printf("[1] Inicializa Vetor\n");
        printf("[2] Mostra o vetor atual\n");
        printf("[3] Bubble Sort\n");
        printf("[4] Insertion Sort\n");
        printf("[5] Selection Sort\n");
        printf("[6] Counting Sort\n");
        printf("[7] Merge Sort\n");
        printf("[8] Quick Sort\n");
        printf("[9] Heap Sort\n");

        scanf("%d", &command);

        switch (command)
        {
        case 0:
            return 0;

        case 1:
            init_array(array, SIZE);
            break;

        case 2:
            print_array(array, SIZE);
            break;

        case 3:
            bubble_sort(array, SIZE);
            break;

        case 4:
            insertion_sort(array, SIZE);
            break;

        case 5:
            selection_sort(array, SIZE);
            break;

        case 6:
            counting_sort(array, SIZE);
            break;

        case 7:
            merge_sort(array, 0, SIZE - 1);
            break;

        case 8:
            quick_sort_Hoare(array, 0, SIZE - 1);
            break;

        case 9:
            heap_sort(array, SIZE);
            break;

        default:
            printf("Insira um comando valido!\n");
            break;
        }
    }

    return 0;
}