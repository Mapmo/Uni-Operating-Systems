#!/bin/bash

function print_help {
	echo "Script aiming to create the required environment for chrooting"
	echo "Synonpsis:"
	echo $'\t'"chroot.env [OPTIONS]"
	echo "Options:"
	echo $'\t'"-d DIRECTORY - selects the destination directory for chrooting (if not specified . is selected)"
	echo $'\t'"-t FILE ... - copies the target file/directory using its absolute pathname to the -d directory and does the same for the dependencies of TARGET1 (/bin/bash does not need to be specified)"
	echo $'\t'"-h, --help - prints this help section"
}

function quit {
	if [ $# != 2 ]; then
		quit "Wrong amount of parameters passed to quit"
		exit 3
	fi
	echo $1
	exit $2
}

function extract_ldd {
	if [ $# != 1 ]; then
		quit "Wrong amount of paramters passed to extract_ldd" 3
	fi

	echo $1 >> $ldd_list

	deps=$(ldd $1 | awk '{print $3}' | xargs)
	
	if [ "$deps" == 'statically linked' ]; then
		return 0
	fi

	for i in $deps; do
		extract_ldd $i
	done
}

target_active=false 	#flag used to indicate that parameter -t was given
dir_active=false	#flag used to indicate that parameter -d was given
dir_name="$(pwd)"	#the name of the directory where the chroot will happen
target_files="/bin/bash" #the files that the user needs in the chroot directory

#The following loop will parse the parameters, apologies for the chaotic order of cases, but it was needed to match the logic of the script
for i in "$@"; do
	if [ $dir_active == true ]; then   #required at top because after -d it is mandatory to pass dirname
		dir_name="$i"
		dir_active=false
	elif [[ $i =~ ^- ]]; then 	#required before the check for -t filename because it ends the streak of -t filenames
		case "$i" in
			-h | --help)
				print_help
				target_active=false
				;;
			-d)	dir_active=true
				target_active=false
				;;
			-t) 	target_active=true
				;;
			*) 	quit "Unknown parameter name: $i" 1
		esac
	elif [ $target_active == true ]; then
		if [[ "$i" =~ ^- ]]; then
			quit "Wrong attribute passed to -t: $i" 1
		fi
		target_files="${target_files} ${i}"
	else
		quit "No parameter name specified $i" 1
	fi
done

if [ $dir_active == true ]; then
       	quit "Missing -d value" 1
fi

if [ ! -d $dir_name ]; then
	mkdir -p "$dir_name"
fi

#first validate that all the targets exist
for i in $target_files; do
	if [ ! -e $i ]; then
		quit "No file $i found" 2
	fi
done

ldd_list=$(mktemp)
for i in $target_files; do
	if [ -d $i ]; then
		for j in $(find $i -type f); do
			extract_ldd $j 
		done
	else
		extract_ldd $(dirname $i)/$(basename $i) 
	fi
done

while read dep_name; do 
	dep_new_path=$(dirname $dep_name | tail -c +2)
	mkdir -p $dep_new_path
	cp $dep_name $dep_new_path
done < <(sort -u $ldd_list)

rm $ldd_list

