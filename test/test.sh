#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

uname=`uname`

echo run without loading traceopen lib
rm -f testfile
unset LD_PRELOAD
unset DYLD_INSERT_LIBRARIES
./opentest | tee > notrace

echo -------------------------
echo run with traceopen lib loaded
rm -f testfile
if [[ $uname == "Linux" ]]; then
    source ../linuxenv
elif [[ $uname == "Darwin" ]]; then
    source ../darwinenv
fi
./opentest | tee > withtrace

echo -------------------------

diff -u notrace withtrace

if [ $? ]; then
    echo "Output is the same"
else
    echo "Error!!!"
fi

