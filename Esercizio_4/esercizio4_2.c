/*** Esercizio 4.2 - Scattering
 *
 * Sviluppare un programma MPI che utilizza P processori, il processo 
 * con rank 0 in MPI_COMM_WORLD suddivide un array di float tra tutti i processi 
 * in MPI_COMM_WORLD.
 *
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{

    int rank, size;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float arrayin[size];
    float arrayout;

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime(); // Avvio tempo

    if (rank == 0)
    {

        for (int i = 0; i < size; i++)
        {
            arrayin[i] = i + 1;
        }

        printf("Processo [%d] ha inviato l'array: [", rank);
        for (int i = 0; i < size; i++)
        {
            printf("%.2f ", arrayin[i]);
        }
        printf("]\n");
    }
    MPI_Scatter(arrayin,1,MPI_FLOAT,&arrayout,1,MPI_FLOAT,0,MPI_COMM_WORLD);

    printf("Processo [%d] ha ricevuto dal Processo [%d] il valore %.2f\n", rank, 0, arrayout);

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if(rank == 0){ // Il nodo Master scrive su stdout il tempo
        printf("\n-------------------------------------------*\n");
        printf("Tempo in ms = %f\n", end-start);
        printf("-------------------------------------------*\n");
    }

    MPI_Finalize();
}