/*** Esercizio 2.3
 *** Sviluppare la funzionalità di Reduce nella libreria mycollective , in grado di
 *** supportare gli operatori di massimo, minimo e media di un array di interi
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "mycollective.h"

int main(int argc, char **argv)
{

    int rank, size;
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

    //BlockingReduce(rank, size, MPI_INT, status);
    NotBlockingReduce(rank, size, MPI_INT, status,request);

    MPI_Finalize();
}