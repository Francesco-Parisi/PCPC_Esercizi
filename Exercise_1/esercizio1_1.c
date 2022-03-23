/*** Esercizio 1.1 
 *** Scambio di un valore intero tra due processi MPI
 ***/

#include <stdio.h>
#include "mpi.h"

int main(int argc, char** argv){

    int rank, size, val, len;
    MPI_Status status;
    
    MPI_Init(&argc,&argv);
    
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    MPI_Get_processor_name(name, &len);
    
    if(rank==0){
        printf("Inserisci il valore che il Processo [%d] deve inviare:\n", rank);
        scanf("%d", &val);
        MPI_Send(&val,1,MPI_INT,1,1,MPI_COMM_WORLD);
        printf("- Processore %d ha inviato %d al processo %d\n", rank, val, 1);
        MPI_Recv(&val,1,MPI_INT,1,2,MPI_COMM_WORLD,&status);
        printf("- Processore %d ha ricevuto %d dal processo %d\n", rank, val, 1);
    
    } else if(rank==1){
        MPI_Recv(&val,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
        printf("- Processore %d ha ricevuto %d dal processo %d\n", rank, val, 0);
        val=2;
        MPI_Send(&val,1,MPI_INT,0,2,MPI_COMM_WORLD);
        printf("- Processore %d ha inviato %d al processo %d\n", rank, val, 0);
    }


    MPI_Finalize();

}

