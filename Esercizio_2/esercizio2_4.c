/*** Esercizio 2.4
 * 
 * Modificare la libraria mycollective al fine di realizzare le operazioni di gather,
 * scatter, broadcast, e di reduce in modalità non bloccante. Notare che tali
 * operazioni coinvolgono più processi di conseguenza bisogna fornire nella
 * libreria anche un operazione di wating di un array di MPI_Request, si
 * possono utilizzare le operazioni offerte da MPI.
 * 
 * ***/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "mycollective.h"

int main(int argc, char **argv)
{

    int rank, size;
    double start, end;

    MPI_Status status;
    MPI_Request request;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size < 2)
    {
        printf("Devi utilizzare almeno due processi per eseguire il programma!\n");
        MPI_Finalize();
        exit(0);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    // NotBlockingBroadcasting(rank, 10, size, MPI_INT, status, request);
    // NotBlockingScatter(rank, size, MPI_INT,status, request);
    // BlockingReduce(rank, size, MPI_MIN, MPI_INT, status);
    NotBlockingReduce(rank, size, MPI_MAX, MPI_INT, status, request);

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0)
    {
        printf("\n-------------------------------------------*\n");
        printf("Tempo in ms = %f\n", end - start);
        printf("---------------------------------------------*\n");
    }

    MPI_Finalize();
}