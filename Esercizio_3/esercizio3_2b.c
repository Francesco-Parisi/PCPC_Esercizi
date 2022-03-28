/*** Esercizio 3.2 - Ring 2 Buffered
 *
 * Sviluppare lo stesso programma ma sfruttando la
 * comunicazione bloccante e le differenti modalità
 * di comunicazione
 * 
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, size, next, prev, value, outrank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;
    MPI_Request request;

    int sum = rank;
    next = (rank + 1) % size;
    prev = (rank + size - 1) % size;

    if (size < 2)
    {
        printf("Devi utilizzare almeno 2 processi per eseguire il programma!\n");
        MPI_Finalize();
        exit(0);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Bsend(&rank, 1, MPI_INT, next, 111, MPI_COMM_WORLD);

    for (int i = 0; i < size - 1; i++)
    {
        MPI_Recv(&outrank, 1, MPI_INT, prev, 111, MPI_COMM_WORLD, &status);
        sum += outrank;
        if (outrank != next)
        {
            MPI_Send(&outrank, 1, MPI_INT, next, 111, MPI_COMM_WORLD);
        }
    }
    printf("Processo [%d] ha ricevuto rank %d dal Processo [%d]\n", rank, outrank, next);
    fflush(stdout);
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == size-1)
    {
        printf("Somma Totale (0-%d): %d\n", size-1,sum);
    }

    MPI_Finalize();
    return 0;
}