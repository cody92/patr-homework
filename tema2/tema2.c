#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <math.h>

/*
 * Sem[0] - semafor Pompa libera
 * Sem[1] - semafor Terminare alimentare
 */
sem_t Sem[2];

void* achitare(int);
void* alimentare(int);

void* (*tasks[])(int) = { achitare, alimentare };

int main(void) {
	pthread_t Task[15];
	pthread_attr_t attr;
	int i, val;

	if (!sem_init(Sem, 1, 5)) {
	} else {
		printf("Eroare la initializarea semaforului Pompa libera \n");
	}

	if (!sem_init(Sem + 1, 1, 0)) {
	} else {
		printf("Eroare la initializarea semaforului Terminat alimentare \n");
	}

	pthread_attr_init(&attr);

	for (i = 0; i < 15; i++) {
		sleep(1);
		if (i != 0) {
			if (pthread_create(Task + i, &attr, (void*) (*(tasks + 1)),
					(void*) i) != 0) {
				perror("pthread_create");
				return EXIT_FAILURE;
			}
		} else {
			if (pthread_create(Task + i, &attr, (void*) (*(tasks)), (void*) i)
					!= 0) {
				perror("pthread_create");
				return EXIT_FAILURE;
			}
		}
	}
	for (i = 0; i < 15; i++) {
		if (pthread_join(*(Task + i), NULL) != 0) {
			perror("pthread_join");
			return EXIT_FAILURE;
		}
	}

	/**
	 * Distrugem semafoarele
	 */
	sem_destroy(Sem + 1);
	sem_destroy(Sem);
	exit(0);
}

void* achitare(int v) {
	while (1) {
		sem_wait(Sem + 1);
		printf("\nTest achitare\n");
		sleep(8);
		sem_post(Sem);
	}
}

void* alimentare(int v) {
	sem_wait(Sem);
	printf("\nTest alimentare %d \n", v);
	sem_post(Sem + 1);
}

