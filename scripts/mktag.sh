#!/bin/bash

SRCPATH=$(realpath ${BASH_SOURCE[0]})
SRCDIR="$( cd "$( dirname "${SRCPATH}" )" && pwd )"

if [[ $# != 1 ]]; then
    echo "Usage: mktag.sh <all file list>"
    exit 1
fi

LISTFILE=cscope.files

${SRCDIR}/cleanpath.rb < $1 | grep -v '/$' | sort -u > $LISTFILE
cscope -b
ctags -L $LISTFILE
