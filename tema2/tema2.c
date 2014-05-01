#include <stdio.h>

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#define NUMAR_SEMAFOARE 5
#define NUMAR_SEM_CASE 3
#define NUMAR_TASKURI 15
/*
 * Luam 5 semafoare pentru 5 pompe si un semafor pentru casa
 */
sem_t Sem[NUMAR_SEMAFOARE], casa[NUMAR_SEM_CASE];
int PlataCasa, numarCurent = 0;

void* achitare();
void* alimentare(int);

void* (*tasks[])(int) = { achitare, alimentare };

int main(void) {
	pthread_t Task[NUMAR_TASKURI];
	pthread_attr_t attr;
	int i;

	for (i = 0; i < NUMAR_SEMAFOARE; i++) {
		if (!sem_init(Sem + i, 1, 1)) {
		} else {
			printf("Eroare la initializarea semaforului pentru poma %d! \n", i
					+ 1);
		}
	}

	for (i = 0; i < NUMAR_SEM_CASE; i++) {
		if (!sem_init(casa + i, 1, 0)) {
		} else {
			printf("Eroare la initializarea semaforului Casa libera \n");
		}
	}

	pthread_attr_init(&attr);
	for (i = 0; i < NUMAR_TASKURI; i++) {
		if (i < NUMAR_TASKURI-1) {
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
	for (i = 0; i < NUMAR_TASKURI; i++) {
		if (pthread_join(*(Task + i), NULL) != 0) {
			perror("pthread_join");
			return EXIT_FAILURE;
		}
	}

	/**
	 * Distrugem semafoarele
	 */
	for (i = 0; i < NUMAR_SEMAFOARE; i++) {
		sem_destroy(Sem + i);
	}
	for (i = 0; i < NUMAR_SEM_CASE; i++) {
		sem_destroy(casa + i);
	}
	exit(0);
}

void* achitare() {
	while (numarCurent < NUMAR_TASKURI-1) {
		sem_wait(casa);
		sem_post(casa + 1);
		sem_wait(casa + 2);
		printf("\nAchitare pompa %d:\n", PlataCasa+1);
		sleep(5);
		numarCurent++;
		sem_post(Sem + PlataCasa);
	}
}

void* alimentare(int v) {
	int pompa = 0, test;
	do {
		sem_getvalue(Sem + pompa, &test);
		pompa++;
		if (pompa == NUMAR_SEMAFOARE) {
			pompa = 0;
		}
	} while (test == 0);

	if (pompa == 0) {
		pompa = NUMAR_SEMAFOARE - 1;
	} else {
		pompa--;
	}
	sem_trywait(Sem + pompa);
	printf("\nAlimentare pompa %d in valoare de %d \n", pompa + 1, v);
	sleep(v);
	sem_post(casa);
	sem_wait(casa + 1);
	PlataCasa = pompa;
	sem_post(casa + 2);

}

