#!/bin/bash

#the idea of this shell script is to see the hexdump of the two files and easily check whether the sorting was successful

if (( $# != 2 ))
then
	exit 1
fi

echo "${1} has hex dump:"
xxd "${1}" | cut -d ' ' -f 2- | egrep -o "([0-9a-f]{4} ?){2}"

echo

echo "${2} has hex dump:"
xxd "${2}" | cut -d ' ' -f 2- | egrep -o "([0-9a-f]{4} ?){2}"

