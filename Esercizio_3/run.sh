#!/bin/bash

#####################################
# Author: Francesco 
# Version: v1.0.0
# Description: Compila ed esegue il codice.
#####################################

printf "\nCompilazione in corso...\n"
mpicc $1.c -o $1
if [ $? -eq 0 ]; then
      sleep 3  
      clear
      printf "\nCompilazione effettuata\n"
else
   echo "Errore imprevisto!"
fi
mpirun --allow-run-as-root -np $2 $1
