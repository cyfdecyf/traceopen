#!/bin/bash

# Start the script and create a unique opened file list

SRCPATH=$(realpath ${BASH_SOURCE[0]})
SRCDIR="$( cd "$( dirname "${SRCPATH}" )" && pwd )"

stap --vp 00001 ${SRCDIR}/traceopen.stp $*

