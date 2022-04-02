/*** Esercizio 4.3 - Gathering
 *
 * Sviluppare un programma MPI che utilizza P processori, il processo con rank 0 in
 * MPI_COMM_WORLD colleziona un insieme di valori float distribuiti tra tutti i processi in
 * MPI_COMM_WORLD.
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{

    int rank, size;
    double start, end;
    float value;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float arrayout[size];

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime(); // Avvio tempo

    value = (float)rank + 1;

    MPI_Gather(&value, 1, MPI_FLOAT, arrayout, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    printf("Processo [%d] ha inviato %.2f al Processo [%d]\n", rank, value, 0);

    if (rank == 0)
    {
        printf("Processo [%d] ha ricevuto l'array: [", rank);
        for (int i = 0; i < size; i++)
        {
            printf("%.2f ", arrayout[i]);
        }
        printf("]\n");
    }

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