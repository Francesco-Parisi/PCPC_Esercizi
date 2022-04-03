#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#define N 10

int main(int argc, char **argv)
{

    int rank, numtasks, tag = 111;
    int buffer[N];
    int size = 0, k = 0;
    double start, end;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;
    MPI_Request request;

    int portion = N / numtasks;
    int rest = N % numtasks;
    int array[numtasks];
    int *outbuffer = (int *)malloc(sizeof(int) * size);
    int tmp;

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    if (rank < rest)
        size = portion + 1;
    else
        size = portion;

    MPI_Send(&size, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);

    if (rank == 0)
    {
        // inizializzo l'array utilizzando il rank di ogni processore
        for (int i = 0; i < N; i++)
        {
            buffer[i] = i + 1;
        }

        for (int i = 0; i < numtasks; i++)
        {
            MPI_Recv(&array[i], 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
        }

        // Ordinamento array size
        for (int i = 0; i < numtasks; ++i)
        {
            for (int j = i + 1; j < numtasks; ++j)
            {
                if (array[i] < array[j])
                {
                    tmp = array[i];
                    array[i] = array[j];
                    array[j] = tmp;
                }
            }
        }

        for (int i = 0; i < numtasks; i++)
        {
            for (int j = 0; j < array[i]; j++)
            {
                MPI_Send(&buffer[k], 1, MPI_INT, i, tag, MPI_COMM_WORLD);
                k++;
            }
        }
    }

    // Gli slave ricevono il buffer
    for (int j = 0; j < size; j++)
    {
        MPI_Recv(&outbuffer[j], N, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        outbuffer[j] = outbuffer[j - 1] + outbuffer[j];
        printf("%d ", outbuffer[j]);
    }
    printf("\n");

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0)
    {
        printf("\n-------------------------------------------*\n");
        printf("Tempo in ms = %f\n", end - start);
        printf("-------------------------------------------*\n");
    }

    MPI_Finalize();
}