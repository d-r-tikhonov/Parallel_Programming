#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define MIN_ARRAY_SIZE   1e4
#define MAX_ARRAY_SIZE   1e8
#define STEP_ARRAY_SIZE  1e7
#define THREAD_THRESHOLD 1e4

typedef struct
{
    int* array;
    int left;
    int right;
} thread_data;

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int comparator(const void* a, const void* b) 
{
    return (*(const int*)a - *(const int*)b);
}

int partition(int* array, int left, int right)
{
    int pivot = array[right];
    int index = left - 1;

    for (int j = left; j < right; ++j)
    {
        if (array[j] <= pivot)
        {
            ++index;
            swap(&array[index], &array[j]);
        }
    }
    
    swap(&array[index + 1], &array[right]);

    return index + 1;
}

void sequential_qsort(int* array, int left, int right) 
{
    if (left < right) 
    {
        int pivot = partition(array, left, right);
        sequential_qsort(array, left, pivot - 1);
        sequential_qsort(array, pivot + 1, right);
    }
}

void* parallel_qsort(void* arg)
{  
    thread_data* data = (thread_data*) arg;

    if ((data->right - data->left) <= THREAD_THRESHOLD)
    {
        sequential_qsort(data->array, data->left, data->right);
    }
    else if (data->left < data->right)
    {
        int pivot = partition(data->array, data->left, data->right);

        thread_data* left_args  = (thread_data*) calloc(1, sizeof(thread_data));
        thread_data* right_args = (thread_data*) calloc(1, sizeof(thread_data));

        *left_args  = (thread_data) {data->array, data->left, pivot - 1};
        *right_args = (thread_data) {data->array, pivot + 1, data->right};

        pthread_t left_thread;
        pthread_create(&left_thread, NULL, parallel_qsort, left_args);

        parallel_qsort(right_args);

        pthread_join(left_thread, NULL);

        free(left_args);
        free(right_args);
    }

    return NULL;
}

bool is_sorted(int* array, size_t size)
{
    for (int index = 1; index < size; ++index)
    {
        if (array[index - 1] > array [index])
        {
            return false;
        }
    }

    return true;
}

int* generate_random_array(int size)
{
    int* array = (int*) calloc(size, sizeof(int));
    if (array == NULL) 
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    for (int index = 0; index < size; ++index)
    {
        array[index] = rand();
    }

    return array;
}

int main(int argc, char* argv[])
{
    srand(time(NULL));

    FILE* file = fopen("03-parallel-sort/output/results.csv", "w");
    if (!file) 
    {
        fprintf(stderr, "Failed to open results.csv\n");
        return 1;
    }

    fprintf(file, "Size,Parallel Time,qsort Time\n");

    for (int array_size = MIN_ARRAY_SIZE; array_size <= MAX_ARRAY_SIZE; array_size += STEP_ARRAY_SIZE) 
    {

        double parallel_time = 0;
        double qsort_time    = 0;

        int* array = generate_random_array(array_size);

        int* array_copy  = (int*) calloc(array_size, sizeof(int));
        memcpy(array_copy, array, array_size * sizeof(int));
    
        clock_t start = clock();
        qsort(array_copy, array_size, sizeof(int), comparator);
        clock_t end = clock();
    
        if (is_sorted(array_copy, array_size))
        {
            qsort_time = (double)(end - start) / CLOCKS_PER_SEC;
            printf("The array of %d elements is sorted by qsort in %f seconds\n", array_size, qsort_time);
        }
        else
        {
            printf("Sorting the array using qsort method failed!\n");
        }
    
        start = clock();
        thread_data args = {array, 0, array_size - 1};
        parallel_qsort(&args);
        end = clock();
    
        if (is_sorted(array, array_size))
        {
            parallel_time = (double)(end - start) / CLOCKS_PER_SEC;
            printf("The array of %d elements is sorted by parallel qsort in %f seconds\n", array_size, parallel_time);
        }
        else
        {
            printf("Sorting the array using parallel qsort method failed!\n");
        }
    
        free(array);
        free(array_copy);

        fprintf(file, "%d,%f,%f\n", array_size, parallel_time, qsort_time);
    }

    fclose(file);
    printf("Results are saved in results.csv file...\n");

    return 0;
}