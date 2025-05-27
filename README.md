Neka program simulira neki dugotrajni posao (slično servisima) koji koristi dvije datoteke: 
u jednu dodaje do sada izračunate vrijednosti (npr. kvadrati slijednih brojeva), a u drugu 
podatak do kuda je stigao. Npr. u obrada.txt zapisuje 1 4 9 … (svaki broj u novi red) a u 
status.txt informaciju o tome gdje je stao ili kako nastaviti. Npr. ako je zadnji broj u 
obrada.txt 100 u status.txt treba zapisati 10 tako da u idućem pokretanju može nastaviti 
raditi i dodavati brojeve.

 Međusobno isključivanje ostvariti za dva procesa/dretve
 međusobnim isključivanjem po Dekkerovom algoritmu.

  Ostvariti program koji simulira tijek rezervacije stolova u nekom
 restoranu. Program na početku treba stvoriti određeni broj dretvi koji
 se zadaje u naredbenom retku. Svaka dretva/proces nakon isteka
 jedne sekunde provjerava ima li slobodnih stolova te slučajno
 odabire jedan od njih. Nakon odabira dretva ulazi u kritični odsječak
 te ponovo provjerava je li odabrani stol slobodan. Ako jest, označava
 stol zauzetim i izlazi iz kritičnog odsječka. U oba slučaja, nakon
 obavljene operacije ispisuje trenutno stanje svih stolova te podatke o
 obavljenoj rezervaciji. Prilikom ispisa za svaki stol mora biti vidljivo
 je li slobodan ili broj dretve/procesa koja je taj stol rezervirala. Broj
 stolova se također zadaje u naredbenom retku. Svaka dretva
 ponavlja isti postupak sve dok više nema slobodnih stolova.
 Program završava kada sve dretve završe
