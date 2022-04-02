/*** Esercizio 4.9
 *
 * Sviluppare un programma MPI che data una matrice di dimensione NxM e
 * P processi, calcola il massimo per ogni riga e il minimo per ogni colonna
 * della matrice utilizzando in modo equo i P processi. Alla terminazione il
 * master scrive su standard output il massimo di ogni riga e il minimo di
 * ogni colonna.
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

    int max, min, value;
    int row = size;
    int column = size;
    int a[row][column], maxbuffer[row], minbuffer[column];

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
                a[i][j] = rand() % 100;
            }
        }

        printf("\nProcesso [%d] ha inviato l'array:\n", rank);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < column; j++)
            {
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    MPI_Bcast(a, row * column, MPI_INT, 0, MPI_COMM_WORLD);

    min = a[0][rank];
    for (int i = 0; i < row; i++)
    {
        if (min > a[i][rank])
            min = a[i][rank];
    }

    max = a[rank][0];
    for (int i = 0; i < column; i++)
    {
        if (max < a[rank][i])
            max = a[rank][i];
    }

    MPI_Gather(&min, 1, MPI_INT, minbuffer, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&max, 1, MPI_INT, maxbuffer, 1, MPI_INT, 0, MPI_COMM_WORLD);


    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0)
    {
        printf("\nIl Processo [%d] ha ricevuto per ogni riga e colonna:\n", rank);
        fflush(stdout);

        for (int i = 0; i < size; i++)
        {
            printf("Colonna %d - Minimo: %d, Riga %d - Massimo: %d\n", i, minbuffer[i], i, maxbuffer[i]);
        }

        printf("\n-------------------------------------------*\n");
        printf("Tempo in ms = %f\n", end - start);
        printf("-------------------------------------------*\n");
    }

    MPI_Finalize();
}