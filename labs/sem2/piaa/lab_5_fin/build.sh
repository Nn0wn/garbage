#!/bin/bash
cd ./build
cmake .. -Dmain=ON -Dshift=ON -Dcmp=ON -Dtest=ON

if [ ! -f "$Makefile" ]; then
    echo "==================CMake OK==================="
    make
fi
if [ ! -f "$test" ]; then
    echo "===============Launching tests==============="
    ./lab_5_test
fi
