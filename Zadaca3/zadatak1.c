#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
int *ULAZ, *BROJ, brSt, brDr, *REZ;

// Funkcija koja pronalazi najveći broj u nizu BROJ
int max(){
	int najv = BROJ[0];
	for(int i = 0; i<brSt; i++){
		if(BROJ[i] > najv){
			najv = BROJ[i];
		}
	}
	return najv;
}

// Provjerava jesu li svi stolovi zauzeti
int sve_zauzeto(){
	for(int i = 0; i<brSt; i++){
		if(REZ[i] == -1){
			return 0;
		}
	}
	return 1;
}

// Ulazak u kritični odsječak koristeći algoritam za međusobno isključivanje
void udji_u_kriticni_odsjecak(int i){
	ULAZ[i] = 1;  // označi da dretva želi ući u kritični odsječak
	BROJ[i] = max() + 1;  // dodijeli broj veći od svih postojećih
	ULAZ[i] = 0;  // oznaka da je završila s izborom broja

	// čekaj dok su druge dretve u kritičnom odsječku ili imaju prioritet
	for(int j = 0;j< brSt-1;j++){
		while(ULAZ[j]!=0){}  // čekaj dok druga dretva bira broj
		while(BROJ[j] != 0 && (BROJ[j] < BROJ[i] || (BROJ[j] == BROJ[i] && j<i))){};
	}
}

// Izlazak iz kritičnog odsječka, oznaka da je dretva završila
void izadji_iz_kriticnog_odsjecka(int i){
	BROJ[i] = 0;
}

// Funkcija koju dretva izvršava da pokuša rezervirati stol
void *provjeri_stol(void *pdr){
	if(sve_zauzeto()){
		pthread_exit("svi su stolovi zauzeti");
	}
	int dr =*((int *)pdr);

	int rand_tbl = rand() % brSt;  // nasumično odaberi stol
	printf("Dretva %d: pokusavam rez stol %d\n", dr + 1, rand_tbl + 1);
	udji_u_kriticni_odsjecak(rand_tbl);  // uđi u kritični odsječak za odabrani stol

	char stanje[brSt];
	for(int i = 0; i<brSt; i++){
		stanje[i] = (REZ[i] == -1) ? '-' : REZ[i] + 1 + '0';  // prikaži tko je zauzeo stol
	}
	stanje[brSt] = '\0';

	if(REZ[rand_tbl] == -1){  // ako je stol slobodan
		REZ[rand_tbl] = dr;  // rezerviraj stol
		for(int i = 0; i<brSt; i++){
			stanje[i] = (REZ[i] == -1) ? '-' : REZ[i] + 1 + '0';
		}
		stanje[brSt] = '\0';
		printf("Dretva %d: rezerviram stol %d, stanje: %s\n", dr + 1, rand_tbl + 1, stanje);
	}
	else{
		printf("Dretva %d: neuspjela rezervacija stola %d, stanje: %s\n", dr + 1, rand_tbl + 1, stanje);
	}
	izadji_iz_kriticnog_odsjecka(rand_tbl);  // izlazak iz kritičnog odsječka
}

        void main(){
	printf("br dretvi: ");
	scanf("%d", &brDr);
	printf("br st: ");
	scanf("%d", &brSt);
	fflush(stdin);
	pthread_t thread_id;

	// Alociraj memoriju za nizove
	REZ = (int*)malloc(sizeof(int)*brSt);
	BROJ = (int*)malloc(sizeof(int)*brSt);
	ULAZ = (int*)malloc(sizeof(int)*brSt);

	// Inicijaliziraj nizove
	for(int i = 0; i<brSt;i++){
		REZ[i] = -1;  // stolovi su slobodni
		BROJ[i] = 0;
		ULAZ[i] = 0;
	}

	// Dok nisu svi stolovi zauzeti, pokreći dretve za rezervaciju
	while(!sve_zauzeto()){
		for(int i = 0; i<brDr; i++){
			int * pdr = (int * ) malloc (sizeof(int));
			*pdr = i;
			pthread_create(&thread_id, NULL, provjeri_stol, pdr);
		}
	}

	// Čekaj da sve dretve završe
	for(int i = 0; i<brDr; i++){
		pthread_join(thread_id, NULL);
	}

	// Oslobodi memoriju
	free(REZ);
	free(BROJ);
	free(ULAZ);
}
