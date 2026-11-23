#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_COUNT 4
#define N 1000000L

typedef struct {
    int id;
    long start;
    long end;
    long result;
} Task;

void *calculate_sum(void *arg)
{
    Task *task = arg;
    long sum = 0;

    for (long i = task->start; i <= task->end; i++) {
        sum += i;
    }

    task->result = sum;
    return NULL;
}

int main(void)
{
    pthread_t threads[THREAD_COUNT];
    Task tasks[THREAD_COUNT];

    long chunk = N / THREAD_COUNT;

    for (int i = 0; i < THREAD_COUNT; i++) {
        tasks[i].id = i + 1;
        tasks[i].start = i * chunk + 1;
        tasks[i].end = (i == THREAD_COUNT - 1) ? N : (i + 1) * chunk;
        tasks[i].result = 0;

        if (pthread_create(&threads[i], NULL, calculate_sum, &tasks[i]) != 0) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }

    long parallel_result = 0;
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
        printf("Thread %d: %ld\n", tasks[i].id, tasks[i].result);
        parallel_result += tasks[i].result;
    }

    long expected_result = N * (N + 1) / 2;

    printf("\nParallel result: %ld\n", parallel_result);
    printf("Expected result: %ld\n\n", expected_result);
    printf("Correct: %s\n", (parallel_result == expected_result) ? "YES" : "NO");

    return EXIT_SUCCESS;
}
