/*** Esercizio 4.8
 *
 * Sviluppare un programma MPI che data una matrice di dimensione NxM e
 * P processi, calcola il massimo per ogni riga della matrice utilizzando
 * in modo equo i P processi. Alla terminazione il master scrive su standard
 * output il massimo di ogni riga.
 *
 * ***/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv)
{

    int rank, size;
    double start, end;

    srand((unsigned)time(NULL) + rank);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int max;
    int row = size;
    int column = 5;
    int *matrix = (int *)malloc(sizeof(int) * row * column);
    int *proc = malloc(sizeof(int) * column);
    int maxbuff[size];

    if (size < 2)
    {
        printf("Devi utilizzare almeno due processi per eseguire il programma!\n");
        MPI_Finalize();
        exit(0);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    if (rank == 0)
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < column; j++)
            {
                matrix[i * column + j] = rand() % 100;
            }
        }

        printf("\nProcesso [%d] ha inviato la matrice:\n", rank);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < column; j++)
            {
                printf("%d ", matrix[i * column + j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    MPI_Scatter(matrix, column, MPI_INT, proc, column, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Processo [%d] ha ricevuto: ", rank);

    max = proc[0];
    for (int i = 0; i < column; i++)
    {
        printf("%d ", proc[i]);
        if (max < proc[i])
            max = proc[i];
    }
    printf("\n");

    MPI_Gather(&max, 1, MPI_INT, maxbuff, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0)
    {
        printf("\nIl Processo [%d] ha ricevuto:\n", rank);

        for (int i = 0; i < size; i++)
        {
            printf("Riga %d - Massimo: %d\n", i, maxbuff[i]);
        }

        printf("\n-------------------------------------------*\n");
        printf("Tempo in ms = %f\n", end - start);
        printf("-------------------------------------------*\n");
    }

    MPI_Finalize();
}