#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <math.h>

#define NUMAR_SEMAFOARE 5
/*
 * Luam 5 semafoare pentru 5 pompe si un semafor pentru casa
 */
sem_t Sem[NUMAR_SEMAFOARE], casa[2];
int PlataCasa;

void* achitare();
void* alimentare(int);

void* (*tasks[])(int) = { achitare, alimentare };

int main(void) {
	pthread_t Task[15];
	pthread_attr_t attr;
	int i, val;

	for (i = 0; i < NUMAR_SEMAFOARE; i++) {
		if (!sem_init(Sem + i, 1, 0)) {
		} else {
			printf("Eroare la initializarea semaforului pentru poma %d! \n", i
					+ 1);
		}
	}

	if (!sem_init(casa, 1, 0) || !sem_init(casa + 1, 1, 0)) {
	} else {
		printf("Eroare la initializarea semaforului Casa libera \n");
	}

	pthread_attr_init(&attr);
	for (i = 0; i < 15; i++) {
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
	for (i = 0; i < NUMAR_SEMAFOARE; i++) {
		sem_destroy(Sem + i);
	}
	sem_destroy(casa);
	sem_destroy(casa + 1);
	exit(0);
}

void* achitare() {
	while (1) {
		//sem_wait(casa);
		//sem_post(casa + 1);
		printf("\nAchitare pompa:\n", PlataCasa);
		sleep(5);
		//sem_post(Sem + PlataCasa);
	}
}

void* alimentare(int v) {
	printf("test %d\n",v);
	int pompa = 0;

	while (!sem_trywait(Sem + pompa)) {
		pompa++;
		if (pompa == NUMAR_SEMAFOARE) {
			pompa = 0;
		}
	}
	printf("pompa %d\n", pompa);
	sem_wait(Sem + pompa);
	printf("teste %d\n", pompa);
	/*
	printf("\nAlimentare pompa %d in valoare de %d \n", pompa, v);
	sleep(v);
	sem_post(casa);
	//sem_wait(casa + 1);
	PlataCasa = pompa;*/

}

