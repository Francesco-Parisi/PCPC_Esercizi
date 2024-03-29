/*** Esercizio 4.1 - Broadcasting
 *
 * Sviluppare un programma MPI che utilizza P processori,
 * dove il processo con rank 0 in MPI_COMM_WORLD invia un array di float
 * a tutti gli altri processi in MPI_COMM_WORLD.
 *
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{

    int rank, size, dim = 10;
    double start, end;
    float array[dim];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime(); // Avvio tempo

    if (rank == 0)
    {

        for (int i = 0; i < dim; i++)
        {
            array[i] = i + 1;
        }

        printf("Processo [%d] ha inviato l'array: [", rank);
        for (int i = 0; i < dim; i++)
        {
            printf("%.2f ", array[i]);
        }
        printf("]\n");
    }
    MPI_Bcast(array, dim, MPI_FLOAT, 0, MPI_COMM_WORLD);

    printf("Processo [%d] ha ricevuto dal Processo [%d]: [", rank, 0);
    for (int i = 0; i < dim; i++)
    {
        printf("%.2f ", array[i]);
    }
    printf("]\n");

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if(rank == 0){ // Il nodo Master scrive su stdout il tempo
        printf("\n-------------------------------------------*\n");
        printf("Tempo in ms = %f\n", end-start);
        printf("-------------------------------------------*\n");
    }

    MPI_Finalize();
}