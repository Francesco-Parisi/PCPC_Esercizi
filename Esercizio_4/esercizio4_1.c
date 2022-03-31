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

    int rank, size, dim;
    float *array;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status[size],stat[size];

    if (rank == 0)
    {
        printf("Inserisci dimensione array:\n");
        scanf("%d", &dim);

        array = malloc(dim * sizeof(float));

        for (int i = 0; i < dim; i++)
        {
            array[i] = i;
        }

        printf("Processo [%d] ha inviato l'array: [", rank);
        for (int i = 0; i < size; i++)
        {
            MPI_Send(&dim, 1, MPI_INT, i, 111, MPI_COMM_WORLD);
            MPI_Send(array, dim, MPI_FLOAT, i, 112, MPI_COMM_WORLD);
        }

        for (int i = 0; i < dim; i++)
        {
            printf("%.2f ", array[i]);
        }
        printf("]\n");
    }
    else
    {
        MPI_Recv(&dim, 1, MPI_INT, 0, 111, MPI_COMM_WORLD, &stat[rank]);
        array = malloc(dim * sizeof(float));

        MPI_Recv(array, dim, MPI_FLOAT, 0, 112, MPI_COMM_WORLD, &status[rank]);
        printf("Processo [%d] ha ricevuto dal Processo [%d]: [", rank, 0);
        for (int i = 0; i < dim; i++)
        {
            printf("%.2f ", array[i]);
        }
        printf("]\n");
    }
    free(array);

    MPI_Finalize();
}