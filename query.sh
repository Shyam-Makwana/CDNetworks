#!/bin/bash

# This shell script gives you a CPU usage between start time and end time for a specific IP Address and CPU ID
# It takes name of directory as an argument and prints the usage.

# Check if the number of arguments is equal to 1 i.e. name of directory
if [ ! $# -eq 1 ]
then
    echo "Usage: ./query.sh DATA_PATH"
else
    # Compiles C code
    gcc query_logs.c -o query_logs

    # Executes a C code with text file as an argument
    ./query_logs $1/server_logs.txt
fi