#!/bin/bash

TRACEDIR=./tracedir
PWD=`pwd`

cat $TRACEDIR/* | egrep "^${PWD}.*[ch]$" | egrep -v '/$' | sed -e "s,^${PWD}/,,"

