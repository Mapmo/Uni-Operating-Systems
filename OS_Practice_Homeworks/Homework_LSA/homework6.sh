#!/bin/bash

#Task:
#Write a bash script which will parse a text file with the following format:
#64.31.49.26|2013-07-24-06:51:01|Blocked by pesho
#108.171.243.146|2013-07-24-08:17:01|Blocked by niki
#183.13.241.95|2013-07-24-11:51:00|Blocked by mm
#183.240.177.169|2013-07-24-11:51:01|Blocked by joro
#112.111.172.106|2013-07-24-13:00:00|Blocked by toni
#220.113.166.194|2013-07-24-13:00:01|Blocked by pesho
#82.59.74.82|2013-07-24-13:51:00|Blocked by pesho
#103.6.237.126|2013-07-24-16:51:00|Blocked by misho
#111.250.97.231|2013-07-24-18:34:00|Blocked by lubo
#209.208.27.41|2013-07-24-18:34:00|Blocked by vlado
#201.184.44.184|2013-07-24-18:51:01|Blocked by joro
#79.19.133.36|2013-07-24-20:00:00|Blocked by toni

#The script has to output only the lines that have dates older than 14 days. Sort the output by date. 


#Solution

#safe export as mentioned in the lectures
export PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games"

#validation that there was exactly one input
if (( $# != 1 ))
then
	echo "Wrong output"
	exit 1
fi

#preserving the name of the parameter
declare -r name="${1}"

#validation that the file exists and is readble
if [[ ! -e ${name} || ! -r ${name} ]]
then	
	echo "Invalid input"
	exit 1
fi

#the command itself
#it adds an empty log that represents the date two weeks ago and sorts all the data
#after that, it removes everything under the empty log and that removes the empty log itself via head (because it is at the bottom)

date -d "-14 days" +"|%Y-%m-%d-%H:%M:%S|" | cat - "${name}" | sort -t '|' -k 2,2 | grep "^|[^|]*|$" -B $(wc -l "${name}" | cut -d ' ' -f 1) | head -n -1
