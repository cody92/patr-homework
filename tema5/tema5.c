#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <process.h>

#define FOND_INSUFICIENT  1
#define FOND_OK  2

#define DEPUNERE  1
#define RETRAGERE  2

int ChID;
int PIDProces;
int sumaCont = 0;

struct mesaj {
	int responseCode;
	int suma;
	int tipCerere;
};

void afisareCont(int suma) {
	printf("In cont sunt disponibili %d RON \n", suma);
}

void* Server() {
	struct mesaj *Cmsg, *Rmsg;
	int rcvID;

	Cmsg = (struct mesaj*) malloc(sizeof(struct mesaj));
	Rmsg = (struct mesaj*) malloc(sizeof(struct mesaj));

	PIDProces = getpid();
	ChID = ChannelCreate(0);
	while (1) {
		rcvID = MsgReceive(ChID, Cmsg, sizeof(struct mesaj), NULL);
		afisareCont(sumaCont);
		MsgReply(rcvID, 0, Rmsg, sizeof(struct mesaj));

	}
}

void * depunere(int suma) {
	struct mesaj *Cmsg, *Smsg;
	int cID, srv;

	Cmsg = (struct mesaj*) malloc(sizeof(struct mesaj));
	Smsg = (struct mesaj*) malloc(sizeof(struct mesaj));

	Cmsg->suma = suma;
	Cmsg->tipCerere = DEPUNERE;

	cID = ConnectAttach(0, PIDProces, ChID, _NTO_SIDE_CHANNEL, 0);
	if (cID == -1) {
		perror("\tClient: eroare conectare.\n");
	}
	printf("test1");
	srv = MsgSend(cID, Cmsg, sizeof(struct mesaj), Smsg, sizeof(struct mesaj));
	printf("Suma de %d a fost depusa cu success! \n", suma);
	sumaCont += suma;
	ConnectDetach(cID);
}

int main() {
	pthread_t Fir_Server, Operatii[15];
	pthread_attr_t attr, attrO;
	int i;

	pthread_attr_init(&attr);
	pthread_attr_init(&attrO);

	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE );
	pthread_attr_setdetachstate(&attrO, PTHREAD_CREATE_JOINABLE );

	for (i = 0; i < 15; i++) {
			if (pthread_create(Operatii + i, &attrO, (void*) &depunere, (void*) i)
					!= 0) {
				perror("pthread_create");
				return EXIT_FAILURE;
			}
		}
		for (i = 0; i < 15; i++) {
			if (pthread_join(*(Operatii + i), NULL) != 0) {
				perror("pthread_join");
				return EXIT_FAILURE;
			}
		}


	if (pthread_create(&Fir_Server, &attr, (void*) &Server, NULL) != 0) {
		perror("pthread_create");
		return EXIT_FAILURE;
	}
	if (pthread_join(Fir_Server, NULL) != 0) {
		perror("pthread_join");
		return EXIT_FAILURE;
	}

	for (i = 0; i < 15; i++) {
		if (pthread_create(Operatii + i, &attrO, (void*) &depunere, (void*) i)
				!= 0) {
			perror("pthread_create");
			return EXIT_FAILURE;
		}
	}
	for (i = 0; i < 15; i++) {
		if (pthread_join(*(Operatii + i), NULL) != 0) {
			perror("pthread_join");
			return EXIT_FAILURE;
		}
	}
	pthread_attr_destroy(&attr);
	pthread_attr_destroy(&attrO);
	ChannelDestroy(ChID);
	pthread_exit(NULL);
}

