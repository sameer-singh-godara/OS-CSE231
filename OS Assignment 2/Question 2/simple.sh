#!/bin/bash

# Compile and run the counting C program
gcc -o 2 2.c
./2

# Compile and run the main C program
gcc -o 1 1.c
sudo setcap cap_sys_nice=ep 1
./1

# Run the Python script
/bin/python3 graph.py
