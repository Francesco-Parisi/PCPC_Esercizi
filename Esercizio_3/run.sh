#!/bin/bash

#####################################
# Author: Francesco 
# Version: v1.0.0
# Description: Compila ed esegue il codice.
#####################################

mpicc $1.c -o $1
if [ $? -eq 0 ]; then
   printf "\nCompilazione in corso...\n"

else
   echo "Errore imprevisto!"
fi

sleep 3

mpirun --allow-run-as-root -np $2 $1
