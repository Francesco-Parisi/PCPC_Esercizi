/*** Esercizio 1.3
 *   Dati P processi MPI e un array di valori interi e di lunghezza N, si eseguano le operazioni:
 *   Broadcasting, Gathering, Scatter.
 ***  
 *   Esercizio 1.4
 *   A partire dal punto precedente, generalizzare i programmi al fine di ottenere una libreria
 *   mycollective. Non ci sono vincoli sulla firma delle funzioni e/o su requisiti computazionali.
 ***
 *   Esercizio 1.5
 *   Valutare le prestazioni della libreria realizzata, nell'invio dei tipi di dato MPI_INT e MPI_CHAR,
 *   calcolando i tempi di esecuzione su singola macchina al variare della dimensione di N e del numero 
 *   di processi MPI.
 *
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "mycollective.h"

int main(int argc, char** argv){

    int rank,size;    
    MPI_Status status;
    
    //srand((unsigned)time(NULL));
    
    double start, end;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if(size < 2){
        printf("Devi utilizzare almeno due processi per eseguire il programma!\n");
        MPI_Finalize();
        exit(0);
    }

    MPI_Barrier(MPI_COMM_WORLD); //Tutti i processi sono inizilizzati
    start= MPI_Wtime(); //Avvio tempo

    Broadcasting(rank,10,size,status);
    //Gathering(rank,size,status);
    //Scatter(rank,size,status);

    MPI_Barrier(MPI_COMM_WORLD); //Tutti i processi hanno terminato
    end = MPI_Wtime();

    MPI_Finalize();

    if(rank == 0){ // Il nodo Master scrive su stdout il tempo
        printf("-------------------------------------------*\n");
        printf("Tempo in ms = %f\n", end-start);
        printf("-------------------------------------------*\n");
    }
    return 0;
}
