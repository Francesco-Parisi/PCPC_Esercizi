/*** Esercizio 4.5 - mycollective.c
 *
 * Implementare i punti 1,2,3 utilizzando la libreria mycollective e valutare
 * le prestazioni rispetto alle operazioni collettive di MPI (locale).
 *
 * ***/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mycollective.h"

int main(int argc, char **argv)
{

    int rank, size;
    double start, end;
    float value;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime(); // Avvio tempo
    
    //Broadcasting(rank, size, 10, MPI_FLOAT);
    //Scattering(rank, size, MPI_FLOAT);
    Gathering(rank, size, value, MPI_FLOAT);

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0)
    { // Il nodo Master scrive su stdout il tempo
        printf("\n-------------------------------------------*\n");
        printf("Tempo in ms = %f\n", end - start);
        printf("-------------------------------------------*\n");
    }

    MPI_Finalize();
}