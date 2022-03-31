/*** Esercizio 3.1 - Ring 2 Non Bloccante
 *
 * Dati P processori realizzare un programma MPI dove, ogni processore
 * invia il proprio rank al vicino destro nell'ordine di rank definito
 * da MPI_COMM_WORLD e riceve dal vicino sinistro. La comunicazione
 * del proprio rank deve avvenire in modo non bloccante. Inoltre ogni
 * processo inoltra i messaggi che riceve dal vicino sinistro e somma
 * tutti i valori compreso il suo in una variabile. Il processo termina
 * quando ogni processo ha ricevuto il rank di tutti gli altri e scrive
 * su standard output il valore della somma calcolato.
 *
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, size, next, prev, value, outrank;
    double start, end;

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
    start = MPI_Wtime(); // Avvio tempo

    MPI_Isend(&rank, 1, MPI_INT, next, 111, MPI_COMM_WORLD, &request);

    for (int i = 0; i < size - 1; i++)
    {
        MPI_Irecv(&outrank, 1, MPI_INT, prev, 111, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        sum += outrank;
        if (outrank != next)
        {
            MPI_Isend(&outrank, 1, MPI_INT, next, 111, MPI_COMM_WORLD, &request);
        }
    }
    printf("Processo [%d] ha ricevuto rank %d dal Processo [%d]\n", rank, outrank, next);
    fflush(stdout);

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == size - 1)
    {
        printf("\n-------------------------------------------*\n");
        printf("Somma Totale (0-%d): %d\n", size - 1, sum);
        printf("Tempo in ms = %f\n", end - start);
        printf("---------------------------------------------*\n");
    }

    MPI_Finalize();
    return 0;
}