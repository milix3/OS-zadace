#include <stdio.h>              // Za printf i standardni ulaz/izlaz
#include <signal.h>             // Za rad sa signalima (npr. SIGINT)
#include <sys/types.h>          // Sistemski tipovi podataka (npr. pid_t)
#include <string.h>          
#include <sys/ipc.h>          
#include <stdlib.h>            
#include <unistd.h>            
#include <sys/wait.h>         
#include <sys/stat.h>          
#include <sys/shm.h>            
#include <pthread.h>           


#define SHARED_MEMORY_SIZE (0x6400)

int segment_Id;    
int *sh_mem;      


void brisi(){
    printf("signal primljen gasi sve");
    shmdt(sh_mem);                              // Odspajanje procesa od memorije
    shmctl(segment_Id, IPC_RMID, NULL);         // Brisanje segmenta memorije iz sustava
    exit(0);                                     // Izlazak iz programa
}

void main(){
    // Kreiranje segmenta dijeljene memorije
    segment_Id = shmget(IPC_PRIVATE, SHARED_MEMORY_SIZE, IPC_CREAT | 0660 );
    
    // Povezivanje (mapiranje) memorije na adresni prostor procesa
    sh_mem = (int*) shmat(segment_Id, NULL, 0);
    
    // Beskonačna petlja
    while(1){
        if(fork() == 0){ // Dijete proces
            *sh_mem = 1;                         // Dijete upisuje 1 u memoriju
            printf("%d\n", *sh_mem);             // Ispis vrijednosti iz memorije
            sleep(3);                            // Pauza 3 sekunde
            printf("%d\n", *sh_mem);             // Ponovni ispis (vrijednost može biti promijenjena od roditelja)
            exit(0);                             // Dijete završava
        }

        signal(SIGINT, brisi);                   // Postavljanje funkcije 'brisi' kao handler za Ctrl+C (SIGINT)
        *sh_mem = 2;                             // Roditelj upisuje 2 u memoriju
        printf("%d\n", *sh_mem);                 // Ispis trenutne vrijednosti
        sleep(3);                                // Pauza 3 sekunde
        printf("%d\n", *sh_mem);                 // Ponovni ispis vrijednosti iz memorije
        wait(NULL);                              // Roditelj čeka da dijete završi
    }
}
