#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define THREAD_NUM 4

sem_t semaphore;

void* routine(void* args) {
    sem_wait(&semaphore);
    printf("Hello from thread %d\n", *(int*)args);
    sleep(1);
    sem_post(&semaphore);
    free(args);
	return NULL;
}

int main()
{
    int n = 40;
	if (!n)
        (printf("exit 1"), exit(1));
	else
        (printf("exit 0"), exit(0));
    return 0;
}