#!/bin/bash

# Compile and run the first C program
gcc -o 2 2.c
./2

# Compile and run the second C program
gcc -o 1 1.c
sudo setcap cap_sys_nice=ep 1
./1

# Run the Python script
python3 graph.py
