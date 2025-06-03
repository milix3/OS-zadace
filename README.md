1) Let the program simulate some long-running job (similar to services) that uses two files: add the values ​​calculated so far (e.g. squares of sequential numbers) to one, and the information about where it has reached to the other. For example, in obrada.txt it writes 1 4 9 … (each number on a new line) and in status.txt it writes information about where it left off or how to continue. For example, if the last number in obrada.txt is 100, 10 should be written in status.txt so that it can continue working and adding numbers on the next run.

2) Mutual exclusion is achieved for two processes/threads by mutual exclusion according to Dekker's algorithm.

3) Create a program that simulates the process of reserving tables in a restaurant. At the beginning, the program should create a certain number of threads that are specified on the command line. After one second, each thread/process checks whether there are free tables and randomly selects one of them. After selecting a thread, it enters the critical section and checks again whether the selected table is free. If it is, it marks the table as busy and exits the critical section. In both cases, after the operation is completed, it prints the current state of all tables and information about the reservation made. When printing, it must be visible for each table whether it is free or the number of the thread/process that reserved the table. The number of tables is also specified on the command line. Each thread repeats the same procedure until there are no more free tables. The program ends when all threads have finished

4)  models the operation of the carousel using processes and general (named) semaphores
    hree semaphores are used:
     -one for the number of free seats on the carousel (seats, initially n)
     -one for signaling the start of the ride (start_ride)
     -one to signal the end of the ride (end_ride)
    visitors enter only if there is a free seat (sem_wait)
    the carousel starts only when all places are occupied (place semaphore value == 0)
    after the ride, the carousel signals to the visitors that they can get off, and they then vacate the place (sem_post)
    added a signal handler for sigint so that all processes can end properly
    the main process finally deletes the semaphores (sem_unlink) so that they do not remain in the system

5)  solution of the five philosophers problem using a monitor each philosopher is a thread and in an infinite loop thinks and eats
 I use one mutex semaphore for the critical section
 and an array of semaphores sem[i] for each philosopher separately (as condition variables)
 I store the philosopher's state in an array state[]:
   1 - hungry (o)
   2  - eats (x)
   3 - thinks (O)
when the philosopher becomes hungry, he calls the test() function  which checks if he can eat (if his neighbors don't eat) if he can, the semaphore is set and the philosopher starts eating
 when he finishes, he puts the forks back and tries to wake up his neighbors
