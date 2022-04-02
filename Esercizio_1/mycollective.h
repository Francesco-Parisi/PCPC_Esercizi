#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

// Il Processo con rank 0 invia a tutti i processi 1...P-1 un array.
void Broadcasting(int rank, int num, int size, MPI_Datatype type, MPI_Status status)
{

    int *array = (int *)malloc(sizeof(int) * num);

    if (rank == 0)
    {
        for (int i = 0; i < num; i++)
        {
            array[i] = i + 1;
        }

        for (int i = 0; i < size; i++)
        {
            MPI_Send(array, num, type, i, 111, MPI_COMM_WORLD);
        }
        printf("Processo [%d] ha inviato l'array: [ ", rank);

        for (int i = 0; i < num; i++)
        {
            printf("%d ", array[i]);
        }
        printf("]\n");
    }

    MPI_Recv(array, num, type, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    printf("Processo [%d] ha ricevuto dal Processo [%d]: [", rank, 0);

    for (int i = 0; i < num; i++)
    {
        printf("%d ", array[i]);
    }
    printf("]\n");

    free(array);
}

// Il Processo con rank 0 riceve da tutti i processi 1...P-1 un valore intero, formando un array.
void Gathering(int rank, int size, MPI_Datatype type, MPI_Status status)
{
    int val;
    int *array = (int *)malloc(sizeof(int) * size);

    val = rank;
    MPI_Send(&val, 1, type, 0, 111, MPI_COMM_WORLD);
    printf("Processo [%d] ha inviato %d al Processo [%d]\n", rank, val, 0);
    
    if (rank == 0)
    {
        printf("Processo [%d] ha ricevuto: [ ", rank);
        for (int i = 0; i < size; i++)
        {
            MPI_Recv(&array[i], 1, type, MPI_ANY_SOURCE, 111, MPI_COMM_WORLD, &status);
            printf("%d ", array[i]);

        }
        printf("]\n");
    }
    free(array);
}

// Il Processo con rank 0 invia una porzione di array ad ogni processo in 1...P-1.
void Scatter(int rank, int size, MPI_Datatype type, MPI_Status status)
{
    int val;
    int *array = (int *)malloc(sizeof(int) * size);

    if (rank == 0)
    {

        for (int i = 0; i < size; i++)
        {
            array[i] = i;
        }
        printf("Processo [%d] ha inviato [", rank);
        for (int i = 0; i < size; i++)
        {
            MPI_Send(&array[i], 1, type, i, 111, MPI_COMM_WORLD);
            printf("%d ", array[i]);
        }
        printf("]\n");
    }
    else
    {
        MPI_Recv(&val, 1, type, 0, 111, MPI_COMM_WORLD, &status);
        printf("Processo [%d] ha ricevuto %d dal Processo [%d]\n", rank, val, 0);
    }

    free(array);
}