/*** Esercizio 4.10
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

    int rank, size, tag = 111;
    double start, end;

    srand((unsigned)time(NULL) + rank);

    MPI_Datatype columntype;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int min, max, row, column;
    row = column = size;
    int matrix[row][column], procColumn[column], procRow[row];
    int minbuff[size], maxbuff[size];

    MPI_Type_vector(row, 1, column, MPI_INT, &columntype);
    MPI_Type_commit(&columntype);

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
                matrix[i][j] = rand() % 100;
            }
        }

        printf("\nProcesso [%d] ha inviato la matrice:\n", rank);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < column; j++)
            {
                MPI_Send(&matrix[0][j], 1, columntype, j, tag, MPI_COMM_WORLD);
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    MPI_Scatter(matrix, row, MPI_INT, procRow, row, MPI_INT, 0, MPI_COMM_WORLD);

    // MPI_Barrier(MPI_COMM_WORLD);
    MPI_Recv(procColumn, row, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

    MPI_Reduce(procRow, minbuff, size, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(procColumn, maxbuff, size, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0)
    {
        printf("\nIl Processo [%d] ha ricevuto:\n", rank);
        fflush(stdout);

        for (int i = 0; i < size; i++)
        {
            printf("Processo [%d] - Massimo: %d, Minimo: %d\n", i, maxbuff[i], minbuff[i]);
        }

        printf("\n-------------------------------------------*\n");
        printf("Tempo in ms = %f\n", end - start);
        printf("-------------------------------------------*\n");
    }

    MPI_Type_free(&columntype);
    MPI_Finalize();
}