#!/bin/bash
#A script that is updating packages after a specified interval has passed since the build time of the package

#This is the comparison value in Epoch
# I use 2 weeks because this is an interval long enough for breaking bugs to be fixed
previousDate=$(date --date="2 weeks ago" +"%s")

for i in $(echo n | sudo pacman -Syu | grep ^Packages | cut -d ' ' -f 3-); do 
	
	#The name of the package with the date
	package=$(pacman -Si $(echo $i | sed 's/\(-[^-]*\)\{2\}$//') | egrep "^Name|Build" | cut -d ':' -f 2- | tr $'\n' $'\t')
	
	#Only the name of the package
	name=$(echo $package | awk '{print $1}')
	
	#Only the date of the package
	date=$(echo $package | tail -c +$((${#name} + 2)))
	
	#The date of the package converted to Epoch
	dateEpoch=$(date --date="$date" +"%s")
	
	#If the package is older than the specified date, it will get updated
	if [ $dateEpoch -lt $previousDate ]; then
		yes | sudo pacman -S $name
	fi
done 
