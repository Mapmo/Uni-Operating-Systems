#!/bin/bash

declare src="./text.txt"
declare dic="./dic.txt"

if [[ -f $src ]]; then
	if [[ -f $dic ]]; then
		
		tmp=$( cat $src | sed "s/[^a-zA-ZА-Яа-я]/\n/g"| tr -s "\n"| sed 's/.*/\L&/' | sort -u)

		for word in $tmp; do
			counter=0
			parser=$( tre-agrep -w -"$counter" "$word" $dic | head -n 1)
		
			while (( ${#parser} == 0 )) ; do
				(( counter++ ))
				#echo "vlizame v nested loop"
				parser=$( tre-agrep -w -E "$counter" "$word" $dic | head -n 1)
				#echo "parser is '$parser', when word is '$word' and counter is '$counter'"
			done

			if (( counter != 0 )); then
				echo "${word}:${counter}:${parser}"
			fi
			#echo "nested loop ended"
		done

	else
		echo "file '$src' not found"
	fi
else
	echo "file '$src' not found"
fi

