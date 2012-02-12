#!/bin/bash

SRCPATH=$(realpath ${BASH_SOURCE[0]})
SRCDIR="$( cd "$( dirname "${SRCPATH}" )" && pwd )"

export TRACEDIR=`pwd`/tracedir
mkdir -p $TRACEDIR
source ${SRCDIR}/libenv

# Execute command
$*

unset LD_PRELOAD

