#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#define N 5
#define M 15
#define SEMNAME_MJESTA "mjesta"
//Semafor je mehanizam za kontrolu pristupa zajedničkom resursu u računalnom programu
//Vrtuljak je resurs sa ograničenim kapacitetom
/*Semafor označava koliko ima slobodnih mjesta na vrtuljku.

Svaki put kad posjetitelj želi ući u vrtuljak, on traži "mjesto" od semafora (poziva sem_wait).

Ako ima mjesta, ulazi, a semafor smanjuje broj slobodnih mjesta.

Kad vrtuljak završi vožnju i posjetitelji izađu, on vraća mjesta natrag semaforu (sem_post), dopuštajući novim posjetiteljima da uđu.
Kad netko želi ući, smanji semafor (sem_wait).

Kad netko izađe, poveća semafor (sem_post).

Ako je semafor 0, znači da nema slobodnih mjesta i drugi moraju čekati.


*/
void posjetitelj(int i){
	

	sem_t* mjesta = sem_open(SEMNAME_MJESTA, 0);
	while(1){
		sem_wait(mjesta);
		printf("proces %d je usao u vrtuljak\n", i);
		sleep(3);	
	}	
}

void vrtuljak(){
int br = 0;
	sem_t* mjesta = sem_open(SEMNAME_MJESTA,0);
	int * value = malloc(sizeof(int));
	while(1){
		do{
			sem_getvalue(mjesta, value);			
			printf("broj slobodnih mjesta: %d\n", *value);
			sleep(1);
		}while(*value != 0);
	
	sleep(1);
	printf("vrtuljak se vrti %d\n", br++);
	sleep(3);
	printf("Vrtuljak se zaustavio\n");
	
	for(int i=0;i<N; i++){
	sem_post(mjesta);
	printf("Sjedalo br %d na vrtuljku se ispraznilo!\n", i+1);
	}
	
	}
	
}


void main(){
       
	sem_t* mjesta = sem_open(SEMNAME_MJESTA, O_CREAT, 0644, N);
	
	
	if(fork() == 0){
		vrtuljak();
	}
	
	sleep(1);
	for(int i = 0; i<M;i++){
		if(fork() == 0){
			posjetitelj(i);
		}
	}
	

	for(int i = 0; i<M+1; i++){
		wait(NULL);
	}
	
	sem_destroy(mjesta);

}
  
