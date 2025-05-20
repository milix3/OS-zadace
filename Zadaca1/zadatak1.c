#include <math.h>     
#include <stdio.h>    
#include <sys/types.h> 
#include <signal.h>   
#include <stdlib.h>    
#include <unistd.h>   

int zavrsi = 0;        // Kontrolna varijabla za završetak petlje
int statusBr;          // Trenutni status pročitan iz status.txt

// Handler za SIGINT (prekid s tipkovnice Ctrl+C)
void sigint_funkcija ( int sig ){
	printf("\nOdaberite radnju:\n1) Prekid programa (bez dovrsavanja radnje)\n2) Zavrsi program\n3) Ispisi trenutni status\n4) Nastavi\n");
	int input;
	scanf("%d", &input);
	switch(input){
		case 1:
			printf("\nPrekid programa\n");
			exit(0); // Trenutni izlaz iz programa
			break;
		case 2:
			kill(getpid(), SIGTERM); // sebi šalje signal za završetak
			break;
		case 3:
			kill(getpid(), SIGUSR1); // sebi šalje signal za ispis statusa
			break;
		default:
			break; // Nastavlja s radom ako se unese 4 ili nešto drugo
	}
}

// Handler za SIGTERM (signal za završetak)
void sigterm_funkcija ( int sig ){
	printf("\nSIGTERM primljen, zavrsavanje programa\n");
	zavrsi = 1;  // Postavlja kontrolnu varijablu da prekine petlju
	exit(0);     // Završava program
}

// Handler za SIGUSR1 (koristi se za ispis trenutnog statusa)
void sigusr1_funkcija ( int sig ){
	printf("\nTrenutni status: %d\n", statusBr);
}

// Funkcija koja obrađuje podatke
void proces(FILE *status, FILE *obrada){

	if(statusBr == 0){
		// Ako je status 0, pročitaj zadnji broj iz obrada.txt
		int obradaBr;
		while(!feof(obrada)){
			fscanf(obrada, "%d", &obradaBr);
		}
		// Izračunaj status kao zaokruženi korijen zadnjeg broja
		statusBr = round(sqrt(obradaBr));
	}

	// Oznaka da je obrada u tijeku - zapisuje 0 u status.txt
        fseek(status, 0, SEEK_SET); // Postavi pokazivač na početak datoteke status.txt
        fprintf(status, "%d", 0 );  // Upisuje broj 0 u status.txt (znači: "trenutno se obrađuje")
        fflush(status); // Spremi odmah izmjene u status.txt na disk
        fflush(obrada);  // Spremi odmah izmjene u obrada.txt na disk (ako ih ima)
        sleep(3);  // Čekaj 3 sekunde (simulira da se nešto obrađuje)


// Ažurira status i dodaje novi kvadrat u obrada.txt
     fseek(status, 0, SEEK_SET);     // Vraća pokazivač na početak status.txt da možemo prebrisati staru vrijednost 
    fseek(obrada, 0, SEEK_END);     // Pomakni pokazivač na kraj obrada.txt (da dodamo novi broj na kraj)
    statusBr++;                    // Povećaj status za 1 (idemo na sljedeći broj)
    fprintf(status, "%d", statusBr);   // Upisujemo novi status (novi broj)
    fprintf(obrada, "%d\n", statusBr*statusBr);  // U obrada.txt zapisujemo kvadrat tog broja
    fflush(status);        // Spremi izmjene status.txt odmah
    fflush(obrada);    // Spremi izmjene obrada.txt odmah
    sleep(2);    // Čekaj 2 sekunde (simulacija da se nešto događa)

}

// Glavna funkcija
void main(){

	FILE  *status, *obrada;

	// Otvori status.txt za čitanje
	status = fopen("status.txt", "r");
	// Otvori obrada.txt za čitanje i pisanje
	obrada = fopen("obrada.txt", "r+");

	// Učitaj status iz status.txt
	fscanf(status, "%d", &statusBr);

	// Registriraj signalne funkcije
	signal(SIGTERM, sigterm_funkcija);
	signal(SIGINT, sigint_funkcija);
	signal(SIGUSR1, sigusr1_funkcija);

	// Glavna petlja - radi dok zavrsi nije 1
	while(zavrsi == 0){
		// Ponovno otvori status.txt u načinu za pisanje i čitanje
		status = freopen("status.txt", "w+", status);
		proces(status, obrada); // Obradi sljedeći broj
	}

	// Zatvori datoteke nakon završetka
	fclose(status);
	fclose(obrada);
}