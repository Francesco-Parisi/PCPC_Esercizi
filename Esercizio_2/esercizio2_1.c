/*** Esercizio 2.1 Ring
 * 
 * Dati P processi, il processo con rank i invia un valore intero al processo con rank i+1.
 * Notare che lo schema di comunicazione è ad anello e toroidale, quindi il processo con rank
 * P-1 invia al processo con rank 0. L'esecuzione del programma prevede 10 iterazioni.
 * In ogni iterazione, i processi incrementano il valore ricevuto dal vicino di sinistra con
 * un intero pseudo-casuale compreso tra 0-100. Una particolare iterazione termina quando il
 * valore ricevuto da un processo ha superato una certa soglia S data in input al programma.
 * Il programma al termine delle 10 iterazioni scrive su standard output (rank 0) il numero
 * medio di round di comunicazione necessari per arrivare a convergenza utilizzando P ed S.
 * Notare che tra le varie iterazioni non si deve inizializzare nuovamente il generatore di numeri
 * casuali. Si consiglia di inizializzare i generatori utilizzando il valore del rank.
 * 
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mpi.h>
#include <stdbool.h>

#define S 2000
#define I 10

int main(int argc, char **argv)
{
    int rank, size, next, prev, value;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sum = 0, newsum = 0;
    int count = 0;
    bool flag = false;
    MPI_Status status;

    srand((unsigned)time(NULL) + rank);

    next = (rank + 1) % size;
    prev = (rank + size - 1) % size;

    if (size < 2)
    {
        printf("Devi utilizzare almeno 2 processi per eseguire il programma!\n");
        MPI_Finalize();
        exit(0);
    }

    for (int i = 0; i < I; i++)
    {

        if (rank == 0)
        {
            while (flag != true)
            {
                count++;
                value = rand() % 100 + 1;
                newsum = sum + value;
                MPI_Send(&newsum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
                printf("Processo [%d] ha ricevuto %d e con %d invia la somma %d al Processo [%d]\n", rank, sum, value, sum + value, next);
                fflush(stdout);

                MPI_Recv(&sum, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                if (sum > S)
                {
                    flag = true;
                }
            }
        }
        else
        {
            while (flag != true)
            {
                MPI_Recv(&sum, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                if (sum <= S)
                {
                    value = rand() % 100 + 1;
                    newsum = sum + value;

                    MPI_Send(&newsum, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
                    printf("Processo [%d] ha ricevuto %d e con %d invia la somma %d al Processo [%d]\n", rank, sum, value, newsum, next);
                    fflush(stdout);
                }
                else
                {
                    flag = true;
                    MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                }
            }
        }
    }

    if (rank == 0)
    {
        printf("\nRound Totali: %d\nSomma totale: %d\n", count, sum);
    }

    MPI_Finalize();
    return 0;
}