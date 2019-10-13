#!/bin/bash
	
if (( ${#} != 4 )); then

	echo "wrong amount of arguments"
	exit 1
else

	declare -r log="${1}"
	declare -r eeg="${2}"
	declare -r laring="${3}"
	declare -r dest="${4}"

	if [[ -f "${log}" && -r "${log}" ]]; then
	
		if [[ -f "${eeg}" && -r "${eeg}" ]]; then
	        	
			if [[ -f "${laring}" && -r "${laring}" ]]; then
	
	        		if [[ -d "${dest}" && -w "${dest}" ]]; then
					
	
					#checking for repeating pulses in the .log file
					declare -r repeated=$( cut -d ' ' -f 1 "${log}" | sort | uniq -d)
					if [ -n "${repeated}" ]; then
						echo -e "The following pulses are repeated:\n${repeated}\n-----------------"
					fi
	
	
					#the fragment below calculates in times since Epoch when the .xml file was started
					
					if [ -z "${EEG_TZ}" ]; then
						declare -r EEG_TZ='UTC' #know that by default this will happen, but want to keep my code clear
					fi
					declare -r xmlStartRecDate=$(grep "<StartRecordingDate>" "${eeg}" |sed -r 's/<[^>]*>//g' | sed -r "s-([[:digit:]]*).([[:digit:]]*).-\2/\1/-" )
					#xmlStartRecDate also gets converted to the stupid american model
					declare -r xmlStartRecTime=$( grep "<StartRecordingTime>" "${eeg}" | sed -r 's/<[^>]*>//g' )
					declare -r xmlStart=$( echo $( TZ=":${EEG_TZ}" date --date="$( echo ${xmlStartRecDate} ${xmlStartRecTime} )" +%s.%6N ) )
					
	
					declare -r eegDuration=$( grep "<RecordLength>" "${eeg}" | sed -r 's/<[^>]*>//g' | awk -F: '{ print ($1 * 3600) + ($2 * 60) + $3 }')
					
	
					#the fragment below calculates the beginning of the laring file after the beep signal, its duration 
					#and its ending compared to the beginning of the .xml
	
					declare -r laringDuration=$( soxi -D "${laring}" )
					declare -r laringStart=$( grep "^beep" "${log}" | cut -d ' ' -f 3 )
					declare -r laringStartDif=$(echo "${laringStart} - ${xmlStart}" | bc )
					declare -r laringTotal=$(echo "${laringDuration} + ${laringStartDif}"  | bc )
					
	
					#the fragment below finds which recording started later - .xml or audio
	
					flag=$( echo "${xmlStart} > ${laringStart}" | bc )
					if (( ${flag} == 1 )); then
						declare -r later=${xmlStart}
					else
						declare -r later=${laringStart}
					fi
	
	
					#the fragment below finds which is earlier - the end of the .xml, or the end of the laring
	
					flag=$( echo "${eegDuration} < ${laringTotal}" | bc ) 
					if (( ${flag} == 1 )); then
						declare -r shorter=${eegDuration}
					else
						declare -r shorter=${laringTotal}
					fi
	
					
					declare -r eegRate=$( grep "<SamplingRate>" "${eeg}" | grep -o "[0-9]*" )
					
					
					#extracting the ticks into a separate file ( explained why in documentation )
					declare -r tmpFile=$( mktemp )
					grep "<tick>" "${eeg}" > "${tmpFile}"
	
	
					#reading the .log info
	
					while read name start end; do
						
						interval=$( echo "${end} - ${start}" | bc )
						flag=$( echo "${interval} < 0.2" | bc )
						if (( ${flag} == 1 )); then
	
						        echo "${name} is ${interval} seconds long and therefore it is unsuitable"
							continue
						else
	
	
							#relativeStart/End are used to determine at what time in the .xml and audio file the log happens
							
	
							#checking if log starts before the .xml
							
							
							flag=$( echo "${start} < ${later} " | bc )
							if (( ${flag} == 1 )); then
								echo "${name} starts too early"
								continue
							fi
	                                                xmlRelativeStart=$( echo ${start} - ${xmlStart} | bc )

	
							#checking if log ends after the .xml, note that we sort the logs by ending
	
							xmlRelativeEnd=$( echo "${end} - ${xmlStart}" | bc )
							flag=$( echo "${xmlRelativeEnd} > ${shorter}" | bc )
							if (( ${flag} == 1 )); then
								 echo "${name} continues after XML and so the operation ends"
								 continue
							fi
							
	
							#xmlStartLine/EndLine are the line borders we need to easily extract with sed -n
							
							xmlEndLine=$( echo "${xmlRelativeEnd} * ${eegRate} / 1 + 1" | bc )
							#the fragment below is better but you insisted to use floor
							#xmlStartLine=$( echo "a=${xmlRelativeStart}; b=${eegRate}; if ( a%b ) a*b/1+2 else a*b/1+1" | bc )
                                                        xmlStartLine=$( echo "${xmlRelativeStart} * ${eegRate} / 1 + 1" | bc )
							sed -n "${xmlStartLine},${xmlEndLine}p" "${tmpFile}"  > "${dest}/${name}_eeg.xml"
							
	
							#finding the pulse start time relative to the audio timing
							
							larRelativeStart=$( echo "${start} - ${laringStart}" | bc )
							sox "${laring}" "${dest}/${name}_lar.wav" trim ${larRelativeStart} ${interval}
						fi
	
					done < "${log}"
					rm "${tmpFile}" 2>/dev/null
				else
					echo "Destination ${dest} is an invalid argument"
					exit 2
				fi
	
			else    
	        		echo "File ${laring} is an invalid argument"
				exit 3
			fi
	
		else    
	       		echo "File ${eeg} is an invalid argument"
			exit 4 
		 fi
	
	else
		echo "File ${log} is an invalid argument"
		exit 5
	fi
fi
