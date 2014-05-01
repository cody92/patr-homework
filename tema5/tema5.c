#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <process.h>

#define FOND_INSUFICIENT  2
#define SUCCES  1

#define DEPUNERE  1
#define RETRAGERE  2
#define NUMAR_PROCESE 100

int ChID;
int PIDProces;
int sumaCont = 0;

struct mesajSend {
	int suma;
	int tipCerere;
};

struct mesajReceive {
	int sumaVeche;
	int sumaNoua;
	int responseCode;
};

void afisareCont(int suma) {
	printf("In cont sunt disponibili %d RON \n", suma);
}

void * depunere(int suma) {
	struct mesajSend *ClientMessage;
	struct mesajReceive *ServerResponse;
	int cID, srv;

	ClientMessage = (struct mesajSend*) malloc(sizeof(struct mesajSend));
	ServerResponse = (struct mesajReceive*) malloc(sizeof(struct mesajReceive));

	ClientMessage->suma = suma;
	ClientMessage->tipCerere = DEPUNERE;
	cID = ConnectAttach(0, PIDProces, ChID, _NTO_SIDE_CHANNEL, 0);
	if (cID == -1) {
		perror("\tClient: eroare conectare.\n");
	}
	srv = MsgSend(cID, ClientMessage, sizeof(struct mesajSend), ServerResponse,
			sizeof(struct mesajReceive));
	printf("Suma cont inainte: %d , suma depusa: %d , suma cont dupa: %d   \n",
			ServerResponse->sumaVeche, ClientMessage->suma,
			ServerResponse->sumaNoua);
	ConnectDetach(cID);
}

void * retragere(int suma) {
	struct mesajSend *ClientMessage;
	struct mesajReceive *ServerResponse;
	int cID, srv;

	ClientMessage = (struct mesajSend*) malloc(sizeof(struct mesajSend));
	ServerResponse = (struct mesajReceive*) malloc(sizeof(struct mesajReceive));

	ClientMessage->suma = suma + 100;
	ClientMessage->tipCerere = RETRAGERE;
	cID = ConnectAttach(0, PIDProces, ChID, _NTO_SIDE_CHANNEL, 0);
	if (cID == -1) {
		perror("\tClient: eroare conectare.\n");
	}
	srv = MsgSend(cID, ClientMessage, sizeof(struct mesajSend), ServerResponse,
			sizeof(struct mesajReceive));
	switch (ServerResponse->responseCode) {
	case SUCCES:
		printf(
				"Suma cont inainte: %d , suma retrasa: %d , suma cont dupa: %d   \n",
				ServerResponse->sumaVeche, ClientMessage->suma,
				ServerResponse->sumaNoua);
		break;
	case FOND_INSUFICIENT:
		printf(
				"Nu se poate efectua retragerea!Aveti in cont suma de %d si doriti sa retrageti suma de %d\n",
				ServerResponse->sumaVeche, ClientMessage->suma);
		break;

	}
	ConnectDetach(cID);
}

int main() {
	pthread_t Fir_Server, Operatii[100];
	pthread_attr_t attr, attrO;
	int i, rcvID, numarCurentProcese = 0, test;
	struct mesajSend *ClientMessage;
	struct mesajReceive *ServerResponse;

	ClientMessage = (struct mesajSend*) malloc(sizeof(struct mesajSend));

	pthread_attr_init(&attr);
	pthread_attr_init(&attrO);

	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE );
	pthread_attr_setdetachstate(&attrO, PTHREAD_CREATE_JOINABLE );

	PIDProces = getpid();
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE );

	ChID = ChannelCreate(0);
	if (ChID == -1) {
		perror("\tClient: eroare creare canal.\n");
	}
	for (i = 1; i <= NUMAR_PROCESE; i++) {
		if (i % 10 == 0) {
			if (pthread_create(Operatii + i, &attrO, (void*) &retragere,
					(void*) i) != 0) {
				perror("pthread_create");
				return EXIT_FAILURE;
			}
		} else {
			if (pthread_create(Operatii + i, &attrO, (void*) &depunere,
					(void*) i) != 0) {
				perror("pthread_create");
				return EXIT_FAILURE;
			}
		}
	}
	/*
	for (i = 0; i < NUMAR_PROCESE; i++) {
		if (pthread_join(*(Operatii + i), NULL) != 0) {
			perror("pthread_join eroare");
			return EXIT_FAILURE;
		}
	}
	*/
	while (numarCurentProcese < NUMAR_PROCESE) {
		rcvID = MsgReceive(ChID, ClientMessage, sizeof(struct mesajSend), NULL);

		switch (ClientMessage->tipCerere) {
		case DEPUNERE:
			ServerResponse = (struct mesajReceive*) malloc(
					sizeof(struct mesajReceive));
			ServerResponse->responseCode = SUCCES;
			ServerResponse->sumaVeche = sumaCont;
			sumaCont += ClientMessage->suma;
			ServerResponse->sumaNoua = sumaCont;
			break;
		case RETRAGERE:
			ServerResponse = (struct mesajReceive*) malloc(
					sizeof(struct mesajReceive));
			if (sumaCont - ClientMessage->suma >= 0) {
				ServerResponse->responseCode = SUCCES;
				ServerResponse->sumaVeche = sumaCont;
				sumaCont -= ClientMessage->suma;
				ServerResponse->sumaNoua = sumaCont;
			} else {
				ServerResponse->responseCode = FOND_INSUFICIENT;
				ServerResponse->sumaVeche = sumaCont;
				ServerResponse->sumaNoua = 0;
			}

			break;
		}
		numarCurentProcese++;
		printf("test %d\n", numarCurentProcese);
		MsgReply(rcvID, 0, ServerResponse, sizeof(struct mesajReceive));

	}
	pthread_attr_destroy(&attr);
	pthread_attr_destroy(&attrO);
	ChannelDestroy(ChID);
	pthread_exit(NULL);
}

