/*** Esercizio 4.7
 *
 * Sviluppare un programma MPI che dato un array, A di interi di lunghezza N,
 * utilizza equamente P processori per aggiornare i valori in A. Ogni elemento
 * A[i] è calcoloato utilizzando la seguente operazione:
 * - A[i]=A[i-1]+A[i]+A[i+1], per ogni i, 1...N-2
 * - A[0]=A[N-1]+[0]+A[1], i=0
 * - A[N-1]=A[N-2]+[N-1]+A[0], i=N-1
 * l'array A viene inizializzato nel processo master e gli slave eseguono le
 * operazioni solo sulla propria porzione di array; ogni processo slave invia
 * la sua porzione di array nuovamente al master. Alla terminazione delle
 * ricezioni il processo master scrive su standard output il tempo di esecuzione.
 *
 * ***/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv)
{

    int rank, size;
    double start, end;

    srand((unsigned)time(NULL) + rank);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int a[size];

    if (size < 2)
    {
        printf("Devi utilizzare almeno due processi per eseguire il programma!\n");
        MPI_Finalize();
        exit(0);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();




    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0)
    {
        printf("\n-------------------------------------------*\n");
        printf("Tempo in ms = %f\n", end - start);
        printf("-------------------------------------------*\n");
    }

    MPI_Finalize();
}