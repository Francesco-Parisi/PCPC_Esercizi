#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

// Bloccante - Il Processo con rank 0 invia a tutti i processi 1...P-1 un array.
void BlockingBroadcasting(int rank, int num, int size, MPI_Datatype type, MPI_Status status)
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
    printf("Processo [%d] ha ricevuto dal Processo [%d]: [ ", rank, 0);

    for (int i = 0; i < num; i++)
    {
        printf("%d ", array[i]);
    }
    printf("]\n");

    free(array);
}

// Non Bloccante - Il Processo con rank 0 invia a tutti i processi 1...P-1 un array.
void NotBlockingBroadcasting(int rank, int num, int size, MPI_Datatype type, MPI_Status status, MPI_Request request)
{
    int *array = (int *)malloc(sizeof(int) * num);

    if (rank == 0)
    {
        for (int i = 0; i < num; i++)
        {
            array[i] = i;
        }
        for (int i = 0; i < size; i++)
        {
            MPI_Isend(array, num, type, i, 111, MPI_COMM_WORLD, &request);
        }
        printf("Processo [%d] ha inviato l'array: [ ", rank);
        for (int i = 0; i < num; i++)
        {
            printf("%d ", array[i]);
        }
        printf("]\n");
    }

    MPI_Irecv(array, num, type, 0, 111, MPI_COMM_WORLD, &request);
    printf("Processo [%d] ha ricevuto dal Processo [%d]: [", rank++, 0);

    MPI_Wait(&request, &status);

    for (int i = 0; i < num; i++)
    {
        printf("%d ", array[i]);
    }
    printf("]\n");

    free(array);
}

// Funzione che implementa MPI_Bcast per l'invio di un array dal nodo master
void Broadcasting(int rank, int size, int dim, MPI_Datatype type)
{
    float buffer[dim];

    if (rank == 0)
    {

        for (int i = 0; i < dim; i++)
        {
            buffer[i] = i + 1;
        }

        printf("Processo [%d] ha inviato l'array: [", rank);
        for (int i = 0; i < dim; i++)
        {
            printf("%.2f ", buffer[i]);
        }
        printf("]\n");
    }
    MPI_Bcast(buffer, dim, type, 0, MPI_COMM_WORLD);

    printf("Processo [%d] ha ricevuto dal Processo [%d]: [", rank, 0);
    for (int i = 0; i < dim; i++)
    {
        printf("%.2f ", buffer[i]);
    }
    printf("]\n");
}

// Bloccante Il Processo con rank 0 riceve da tutti i processi 1...P-1 un valore intero, formando un array.
void BlockingGathering(int rank, int size, MPI_Datatype type, MPI_Status status)
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

// Non Bloccante - Il Processo con rank 0 riceve da tutti i processi 1...P-1 un valore intero, formando un array.
void NotBlockingGathering(int rank, int size, MPI_Datatype type, MPI_Status status, MPI_Request request)
{
    int val;
    int *array = (int *)malloc(sizeof(int) * size);

    val = rank;
    MPI_Isend(&val, 1, type, 0, 111, MPI_COMM_WORLD, &request);
    printf("Processo [%d] ha inviato %d al Processo [%d]\n", rank, val, 0);

    if (rank == 0)
    {
        for (int i = 0; i < size; i++)
        {
            MPI_Irecv(&array[i], 1, type, MPI_ANY_SOURCE, 111, MPI_COMM_WORLD, &request);
        }
        MPI_Wait(&request, &status);

        printf("Processo [%d] ha ricevuto: [ ", rank);
        for (int i = 0; i < size; i++)
        {
            printf("%d ", array[i]);
        }
        printf("]\n");
    }
    free(array);
}

// Funzione che implementa MPI_Gather per la ricezione di un array al nodo master
void Gathering(int rank, int size, float value, MPI_Datatype type)
{

    float arrayout[size];
    value = (float)rank + 1;

    MPI_Gather(&value, 1, MPI_FLOAT, arrayout, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    printf("Processo [%d] ha inviato %.2f al Processo [%d]\n", rank, value, 0);

    if (rank == 0)
    {
        printf("Processo [%d] ha ricevuto l'array: [ ", rank);
        for (int i = 0; i < size; i++)
        {
            printf("%.2f ", arrayout[i]);
        }
        printf("]\n");
    }
}

// Bloccante - Il Processo con rank 0 invia una porzione di array ad ogni processo in 1...P-1.
void BlockingScatter(int rank, int size, MPI_Datatype type, MPI_Status status)
{
    int val;
    int *array = (int *)malloc(sizeof(int) * size);

    if (rank == 0)
    {

        for (int i = 0; i < size; i++)
        {
            array[i] = i;
        }
        printf("Processo [%d] ha inviato [ ", rank);
        for (int i = 0; i < size; i++)
        {
            MPI_Send(&array[i], 1, type, i, 111, MPI_COMM_WORLD);
            printf("%d ", array[i]);
        }
        printf("]\n");
    }

    MPI_Recv(&val, 1, type, 0, 111, MPI_COMM_WORLD, &status);
    printf("Processo [%d] ha ricevuto %d dal Processo [%d]\n", rank, val, 0);

    free(array);
}

// Non Bloccante - Il Processo con rank 0 invia una porzione di array ad ogni processo in 1...P-1.
void NotBlockingScatter(int rank, int size, MPI_Datatype type, MPI_Status status, MPI_Request request)
{
    int val;
    int *array = (int *)malloc(sizeof(int) * size);

    if (rank == 0)
    {

        for (int i = 0; i < size; i++)
        {
            array[i] = i;
        }
        printf("Processo [%d] ha inviato [ ", rank);
        for (int i = 0; i < size; i++)
        {
            MPI_Isend(&array[i], 1, type, i, 111, MPI_COMM_WORLD, &request);
            printf("%d ", array[i]);
        }
        printf("]\n");
    }

    MPI_Irecv(&val, 1, type, 0, 111, MPI_COMM_WORLD, &request);

    MPI_Wait(&request, &status);
    printf("Processo [%d] ha ricevuto %d dal Processo [%d]\n", rank, val, 0);

    free(array);
}

// Funzione che implementa MPI_Scatter per la ricezione di un valore dal nodo master ad ogni processo
void Scattering(int rank, int size, MPI_Datatype type)
{
    float arrayin[size];
    float arrayout;
    if (rank == 0)
    {

        for (int i = 0; i < size; i++)
        {
            arrayin[i] = i + 1;
        }

        printf("Processo [%d] ha inviato l'array: [ ", rank);
        for (int i = 0; i < size; i++)
        {
            printf("%.2f ", arrayin[i]);
        }
        printf("]\n");
    }
    MPI_Scatter(arrayin, 1, MPI_FLOAT, &arrayout, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    printf("Processo [%d] ha ricevuto dal Processo [%d] il valore %.2f\n", rank, 0, arrayout);
    fflush(stdout);
}

// Bloccante - Funzione in grado di supportare gli operatori di massimo, minimo e media di un array di interi.
void BlockingReduce(int rank, int size, int op, MPI_Datatype type, MPI_Status status)
{

    int value, sum = 0, min, max;
    int *array = (int *)malloc(sizeof(int) * size);

    srand((unsigned)time(NULL) + rank);

    value = rand() % 100 + 1;
    MPI_Send(&value, 1, type, 0, 111, MPI_COMM_WORLD);
    printf("Processo [%d] ha inviato %d al Processo [%d]\n", rank, value, 0);

    if (rank == 0)
    {
        for (int i = 0; i < size; i++)
        {
            MPI_Recv(&array[i], 1, type, MPI_ANY_SOURCE, 111, MPI_COMM_WORLD, &status);
        }

        printf("Processo [%d] ha ricevuto: [ ", rank);
        for (int i = 0; i < size; i++)
        {
            printf("%d ", array[i]);
        }

        if (op == 0)
        {
            min = array[0];
            for (int i = 0; i < size; i++)
            {
                if (min > array[i])
                    min = array[i];
            }
            printf(" ]\nMinimo: %d\n", min);
        }
        if (op == 1)
        {
            max = array[0];
            for (int i = 0; i < size; i++)
            {
                if (max < array[i])
                    max = array[i];
            }
            printf(" ]\nMassimo: %d\n", max);
        }
        if (op == 2)
        {
            for (int i = 0; i < size; i++)
            {
                sum += array[i];
            }
            float avg = (float)sum / size;
            printf(" ]\nMedia: %.2f\n", avg);
        }
    }
    free(array);
}

// Non Bloccante - Funzione in grado di supportare gli operatori di massimo, minimo e media di un array di interi.
void NotBlockingReduce(int rank, int size, int op, MPI_Datatype type, MPI_Status status, MPI_Request request)
{

    int value, sum = 0, min, max;
    int *array = (int *)malloc(sizeof(int) * size);

    srand((unsigned)time(NULL) + rank);

    value = rand() % 100 + 1;
    MPI_Isend(&value, 1, type, 0, 111, MPI_COMM_WORLD, &request);
    printf("Processo [%d] ha inviato %d al Processo [%d]\n", rank, value, 0);
    fflush(stdout);
    if (rank == 0)
    {
        for (int i = 0; i < size; i++)
        {
            MPI_Irecv(&array[i], 1, type, MPI_ANY_SOURCE, 111, MPI_COMM_WORLD, &request);
        }
        MPI_Wait(&request, &status);

        printf("Processo [%d] ha ricevuto: [ ", rank);
        for (int i = 0; i < size; i++)
        {
            printf("%d ", array[i]);
        }

        if (op == 0)
        {
            min = array[0];
            for (int i = 0; i < size; i++)
            {
                if (min > array[i])
                    min = array[i];
            }
            printf(" ]\nMinimo: %d\n", min);
        }
        if (op == 1)
        {
            max = array[0];
            for (int i = 0; i < size; i++)
            {
                if (max < array[i])
                    max = array[i];
            }
            printf(" ]\nMassimo: %d\n", max);
        }
        if (op == 2)
        {
            for (int i = 0; i < size; i++)
            {
                sum += array[i];
            }
            float avg = (float)sum / size;
            printf(" ]\nMedia: %.2f\n", avg);
        }
    }
    free(array);
}