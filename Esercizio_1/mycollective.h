#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

// Il Processo con rank 0 invia a tutti i processi 1...P-1 un array.
void Broadcasting(int rank, int num,int size, MPI_Status status){
    int *array = (int*)malloc(sizeof(int) * num);
    
    for(int i=0;i<num;i++){
        array[i] = i;
    }

    if(rank == 0){
        for(int i=0;i<size;i++){
            MPI_Send(array,1,MPI_INT,i,111,MPI_COMM_WORLD);
        }
        printf("Processo [%d] ha inviato l'array: [ ",rank); 
        for(int i=0;i<num;i++){
            printf("%d ", array[i]);
        }
        printf("]\n");

    } else{

        MPI_Recv(array,1,MPI_INT,0,111,MPI_COMM_WORLD, &status);
        printf("Processo [%d] ha ricevuto dal Processo [%d]: [",rank++,0); 
        
        for(int i=0;i<num;i++){
            printf("%d ", array[i]);
        }
        printf("]\n");
    }
    free(array);

}

// Il Processo con rank 0 riceve da tutti i processi 1...P-1 un valore intero, formando un array.
void Gathering(int rank,int size, MPI_Status status){
    int val;
    int *array = (int*)malloc(sizeof(int) * size);

    if(rank != 0){    
        val = rank;
        MPI_Send(&val,1,MPI_INT,0,111,MPI_COMM_WORLD);
        printf("Processo [%d] ha inviato %d al Processo [%d]\n", rank, val,0);
    
    } else {
        for(int i=0;i<size-1;i++){
            MPI_Recv(&array[i],1,MPI_INT,MPI_ANY_SOURCE,111,MPI_COMM_WORLD, &status);
        }
        printf("Processo [%d] ha recevuto: [ ", rank);
        for(int i=0;i<size-1;i++){
            printf("%d ",array[i]);
        }
        printf("]\n");

    } 
    free(array);

}

// Il Processo con rank 0 invia una porzione di array ad ogni processo in 1...P-1.
void Scatter(int rank,int size, MPI_Status status){
    int val;
    int *array = (int*)malloc(sizeof(int) * size);

    if(rank == 0){
        
        for(int i=0;i<size;i++){
            array[i] = i;
        }
        printf("Processo [%d] ha inviato [",rank);
        for(int i=1;i<size;i++){
            MPI_Send(&array[i],1,MPI_INT,i,111,MPI_COMM_WORLD);
            printf("%d ", array[i]);
        }
        printf("]\n");

    } else {
        MPI_Recv(&val,1,MPI_INT,0,111,MPI_COMM_WORLD, &status);
        printf("Processo [%d] ha ricevuto %d dal Processo [%d]\n",rank,val,0); 
    }

    free(array);
}