#!/bin/bash

if (( $# != 1 ))
then
	exit 1
fi

xxd "$1" | cut -d ' ' -f 2-10 | tr ' ' $'\n' | egrep -o [0-9a-f]{2}
