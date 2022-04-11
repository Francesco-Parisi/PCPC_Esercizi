/*** Esercizio 2.2
 *
 * Calcolare il massimo, il minimo, la media di array di valori interi utilizzando P
 * processi ed operazioni di comunicazione in modalità bloccante.
 *
 * **/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char **argv)
{

    int rank, size, value, max, min, sum = 0;
    int *array = (int *)malloc(sizeof(int) * size);
    MPI_Status status;
    double start, end;

    srand((unsigned)time(NULL) + rank);
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

    value = rand() % 100 + 1;
    MPI_Send(&value, 1, MPI_INT, 0, 111, MPI_COMM_WORLD);
    printf("Processo [%d] ha inviato %d al Processo [%d]\n", rank, value, 0);

    if (rank == 0)
    {

        for (int i = 0; i < size; i++)
        {
            MPI_Recv(&array[i], 1, MPI_INT, MPI_ANY_SOURCE, 111, MPI_COMM_WORLD, &status);
        }
        min = array[0];
        max = array[0];
        for (int i = 0; i < size; i++)
        {
            sum += array[i];
            if (max < array[i])
                max = array[i];
            if (min > array[i])
                min = array[i];
        }

        printf("Processo [%d] ha ricevuto: [ ", rank);
        for (int i = 0; i < size; i++)
        {
            printf("%d ", array[i]);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0)
    {
        float avg = (float)sum / size;
        printf("]\n\n*----------------------------*\n");
        printf("Massimo: %d\n", max);
        printf("Minimo: %d\n", min);
        printf("Media: %.2f\n", avg);
        printf("Tempo in ms = %f\n", end - start);
        printf("*----------------------------*\n");
    }

    free(array);

    MPI_Finalize();
}