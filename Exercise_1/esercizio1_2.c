/*** Esercizio 1.2
 *** Invio di una stringa (letta da stdin), da parte del 
 *** processo con rank 0, al processo con rank 1.
 ***/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv){

    int rank, size, len=100;
    char *stringa;
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if(rank == 0){
        MPI_Send(&len,1,MPI_INT,1,0,MPI_COMM_WORLD);
        stringa = malloc(len);

        printf("Inserisci una stringa:\n");
        scanf("%s",stringa);

        MPI_Send(stringa,strlen(stringa)+1,MPI_CHAR,1,1,MPI_COMM_WORLD);
        printf("Processore %d ha inviato '%s' al processo %d\n", rank, stringa, 1);
    
    } else if(rank==1) {
        MPI_Recv(&len,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
        stringa = malloc(len);
        MPI_Recv(stringa,len,MPI_CHAR,0,1,MPI_COMM_WORLD,&status);
        printf("Processore %d ha ricevuto '%s' dal processo %d\n", rank, stringa, 0);

    }
    free(stringa);
    
    MPI_Finalize();
}
