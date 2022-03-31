/*** Esercizio 2.1 - Ring
 *
 * Dati P processi, il processo con rank i invia un valore intero al processo con rank i+1.
 * Notare che lo schema di comunicazione è ad anello e toroidale, quindi il processo con rank
 * P-1 invia al processo con rank 0. L'esecuzione del programma prevede 10 iterazioni.
 * In ogni iterazione, i processi incrementano il valore ricevuto dal vicino di sinistra con
 * un intero pseudo-casuale compreso tra 0-100. Una particolare iterazione termina quando il
 * valore ricevuto da un processo ha superato una certa soglia S data in input al programma.
 * Il programma al termine delle 10 iterazioni scrive su standard output (rank 0) il numero
 * medio di round di comunicazione necessari per arrivare a convergenza utilizzando P ed S.
 * Notare che tra le varie iterazioni non si deve inizializzare nuovamente il generatore di
 * numeri casuali. Si consiglia di inizializzare i generatori utilizzando il valore del rank.
 *
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mpi.h>
#include <stdbool.h>

#define S 1500
#define I 10

int main(int argc, char **argv)
{
    int rank, size, next, prev, value;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sum = 0;
    int count = 0;
    bool flag = false;
    double start, end;

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

    MPI_Barrier(MPI_COMM_WORLD); // Tutti i processi sono inizilizzati
    start = MPI_Wtime();         // Avvio tempo

    /*    for (int i = 0; i < I; i++)
        {
    */

    while (!flag)
    {
        if (rank == 0)
        {
            if (count == 0)
            {
                value = rand() % 100 + 1;
                sum = sum + value;
                MPI_Send(&sum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
                printf("Processo [%d] - Random ricevuto: %d, Somma inviata al Processo [%d]: %d\n", rank, value, next, sum);
            }
            count++;
            MPI_Recv(&sum, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            if (sum > S)
            {
                flag = true;
                MPI_Send(&sum, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
            }
            else
            {
                value = rand() % 100 + 1;
                sum = sum + value;
                printf("Processo [%d] - Random ricevuto: %d, Somma inviata al Processo [%d]: %d\n", rank, value, next, sum);
                fflush(stdout);
                MPI_Send(&sum, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
            }
        }
        else
        {

            MPI_Recv(&sum, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);

            if (sum > S)
            {
                flag = true;
                MPI_Send(&sum, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
            }
            else
            {
                value = rand() % 100 + 1;
                sum = sum + value;

                MPI_Send(&sum, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
                printf("Processo [%d] - Random ricevuto: %d, Somma inviata al Processo [%d]: %d\n", rank, value, next, sum);
                fflush(stdout);
            }
        }
    }

    //  }

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0)
    {
        printf("\n--------------------------*\n");
        printf("Threshold: %d\nRound Totali: %d\nMedia Round: %.2f\n", S, count, (float)count / I);
        printf("Tempo in ms = %f\n", end - start);
        printf("--------------------------*\n");
    }

    MPI_Finalize();
    return 0;
}