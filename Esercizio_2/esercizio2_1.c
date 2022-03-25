#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mpi.h>

#define S 200
#define I 10

int main(int argc, char **argv) {

    int rank, size, value=0, sum=0;
    int flag = 0;
    int count = 0;
    MPI_Status status;
    
    srand((unsigned)time(NULL) + rank);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size < 2) {
        printf("Devi utilizzare almeno due processi per eseguire il programma!\n");
        MPI_Finalize();
        exit(0);
    }


    if(rank == 0){
        value = rand()%100+1;
        MPI_Send(&value, 1, MPI_INT, rank+1, 111, MPI_COMM_WORLD);
        printf("Processore [%d] ha inviato %d al Processore [%d]\n", rank, sum, rank+1);
    } else {
        MPI_Recv(&value, 1, MPI_INT, rank-1, 111, MPI_COMM_WORLD,&status);
        int sum =  sum + value;
        value = rand()%100+1;
        printf("Processore [%d] ha ricevuto %d dal Processore [%d] e invia %d\n", rank, value, rank-1, sum);
    }

    MPI_Finalize();

}