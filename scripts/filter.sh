#!/bin/bash

#a program that takes a source directory and copies all its files into a destination directory where all files are in a directory based on their extension

if (( $# == 2 ))
then
	if [[ -d "$1" && -r "$1" && -x "$1" ]]
	then
		if [[ -d "$2" && -w "$2" ]]
           	then    
   			while read a
			do
				echo $a
				mkdir "${2}/${a}"
				find "$1" -type f | egrep "${a}$" | egrep -o "[^/]*$"| xargs -I @ cp "$1"/@ "$2"/"$a"/@
			done< <( find "$1" -type f | egrep -o "\.[^/].*" | sort -u | sed -r "s/^.//" )
           	else
  	             	echo "DEST argument is invalid"
        	fi   
	else
		echo "SRC argument is invalid"
	fi	

else
	echo "Wrong amount of arguments"
fi
