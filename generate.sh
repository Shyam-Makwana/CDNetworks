#!/bin/bash

# This shell script generates the logs for 1000 servers for date 2014-10-31.
# It takes name of directory as an argument and generates a log file inside that directory.
# It creates a directory with name given in argument. After that, it compiles the C code and executes it
# with argument1/server_logs.txt as a argument of C code, where server_logs.txt is a text file
# where logs will be stored for 1000 servers.

# Check if the number of arguments is equal to 1 i.e. name of directory
if [ ! $# -eq 1 ]
then
    echo "Usage: ./generate.sh DATA_PATH"
else
    # Creates a directory
    mkdir $1

    # Compiles C code
    gcc generate_logs.c -o generate_logs

    # Executes a C code with text file as an argument
    ./generate_logs $1/server_logs.txt
fi