#!/bin/bash

#####################################
# Author: Francesco 
# Version: v1.0.0
# Description: Compile e run code.
#####################################

mpicc $1.c -o $1
printf "\nCompilazione in corso...\n"
sleep 3
mpirun --allow-run-as-root -np $2 $1
