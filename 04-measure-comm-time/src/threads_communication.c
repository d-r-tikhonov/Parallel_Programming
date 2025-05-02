#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define DATA_SIZE 32768

int shared_data = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* writer_thread(void* arg) 
{
    clock_t start = clock();

    for (int index = 0; index < DATA_SIZE; ++index) 
    {
        pthread_mutex_lock(&mutex);
        shared_data = index;
        pthread_mutex_unlock(&mutex);
    }

    clock_t end = clock();
    printf("Time of writing to the shared memory: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    return NULL;
}

void* reader_thread(void* arg) 
{
    int data = 0;
    clock_t start = clock();

    for (int index = 0; index < DATA_SIZE; ++index) 
    {
        pthread_mutex_lock(&mutex);
        data = shared_data;
        pthread_mutex_unlock(&mutex);
    }

    clock_t end = clock();
    printf("Reading time from shared memory: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    return NULL;
}

int main() 
{
    pthread_t writer, reader;

    pthread_create(&writer, NULL, writer_thread, NULL);
    pthread_create(&reader, NULL, reader_thread, NULL);

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    return 0;
}