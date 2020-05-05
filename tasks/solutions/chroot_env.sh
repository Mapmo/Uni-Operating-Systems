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

target_active=false 	#flag used to indicate that parameter -t was given
dir_active=false	#flag used to indicate that parameter -d was given
dir_name="$(pwd)"
target_files="/bin/bash"
for i in "$@"; do
	if [ $dir_active == true ]; then
		dir_name="$i"
		dir_active=false
	elif [[ $i =~ ^- ]]; then
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

echo "Targets: $target_files"
echo "Dir: $dir_name"
