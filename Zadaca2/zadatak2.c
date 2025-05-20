#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
int *PRAVO, *ZASTAVICA, pravoId, zastavicaId;
void udi_u_kriticni_odsjecak(int i, int j){
	ZASTAVICA[i] = 1;               // Kažeš da želiš ući u kritični odsječak
	while(!(ZASTAVICA[j] == 0)){    // Čekaš dok drugi proces ne izađe
		if(*PRAVO == j){            // Ako je PRAVO kod drugog procesa, ti se povučeš
			ZASTAVICA[i] = 0;
			while(*PRAVO == j){     // Čekaš dok drugi ne kaže "gotov sam"
			}
			ZASTAVICA[i] = 1;       // Pokušavaš opet
		}
	}
}

void izadji_iz_kriticnog_odsjecka(int i, int j){
	ZASTAVICA[i] = 0;     // Kažeš da više ne želiš ući
	*PRAVO = j;           // Daješ pravo drugom procesu
}

void proc(int i, int j){
	int k, m;
	for(k = 1; k < 5; k++){                     // Pet puta ulazi u kritični odsječak
		udi_u_kriticni_odsjecak(i, j);         // Ulazi u kritični odsječak
		for(m = 1; m < 5; m++){                // Kritični dio koda — nešto što se ne smije preklapati
			printf("i: %d, k: %d, m: %d\n", i, k, m); 
			sleep(1);                         // Pauza da se može vidjeti što se događa
		}
		izadji_iz_kriticnog_odsjecka(i, j);   // Izlazak iz kritičnog odsječka
	}
}


void brisi(){
	shmdt(PRAVO);                     // Odspaja PRAVO iz memorije
	shmdt(ZASTAVICA);                // Odspaja ZASTAVICA iz memorije
	shmctl(pravoId, IPC_RMID, NULL); // Briše segment memorije za PRAVO
	shmctl(zastavicaId, IPC_RMID, NULL); // Briše segment za ZASTAVICA
	exit(0);                         // Završava program
}

void main(){
        pravoId = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0660);
        zastavicaId = shmget(IPC_PRIVATE, sizeof(int) * 2, IPC_CREAT | 0660);
        PRAVO = (int*)shmat(pravoId, NULL, 0);
        ZASTAVICA = (int*)shmat(zastavicaId, NULL, 0);


        if(fork() == 0){
                proc(1,0); //prvo dijete pokrece svoj dio procesa
        }if(fork() == 0){

		proc(0,1); //drugo dijte pokrece suprotni dio procesa

	};
	signal(SIGINT, brisi); // Ako korisnik stisne Ctrl+C, poziva se brisi() za čišćenje
	wait(NULL); //ceka prvo dijete
        wait(NULL); //ceka drugo dijete
        brisi(); //nakon sto oba zavrse, brise memoriju i izlazi
}
