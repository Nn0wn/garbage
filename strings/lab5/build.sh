#!/bin/bash
cd ./build
cmake .. -Dtest=ON -Dmain=ON -Dshift=ON -Dcmp=ON

if [ ! -f "$Makefile" ]; then
    echo "===============CMake OK==============="
    make
fi
if [ ! -f "$lab6test" ]; then
    echo "===============Launching tests==============="
    ./lab6test
fi
