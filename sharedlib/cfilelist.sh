#!/bin/bash

TRACEDIR=./tracedir
PWD=`pwd`

cat $TRACEDIR/* | egrep "^${PWD}.*[ch]$" | sed -e "s,^${PWD}/,," | sort -u

