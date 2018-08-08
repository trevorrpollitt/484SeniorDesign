#!/bin/bash

# might need to allow script permission! "chmod +x script.sh"
# TO RUN: "./script.sh"

set -eu # makes your program exit on error or unbound variable

echo 'Running make script to compile.\n'
echo 'Use "make clean" to remove executable files and -o files.\n'

#gcc -o main main.c aws_lambda.c hx711.c initialize.c pins.c json_stuff.c lights.c pins.c -lcurl -lssl -ljson -lpthread

make

# echo 'Running program now...\n'

# ./main
