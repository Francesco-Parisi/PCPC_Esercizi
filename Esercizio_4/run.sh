#!/bin/bash

#####################################
# Author: Francesco
# Version: v1.0.0
# Description: Compila ed esegue il codice.
#####################################

#-mca btl_vader_single_copy_mechanism none

printf "\nCompilazione in corso"

for i in 1 2 3; do
      sleep 1
      printf "."
done

mpicc $1.c -o $1

if [ $? -eq 0 ]; then
      sleep 1
      clear
      echo "Compilazione effettuata"
      mpirun --allow-run-as-root -np $2 $1 
else
      echo "Errore imprevisto!"
fi
