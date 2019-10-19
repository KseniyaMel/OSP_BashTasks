#!/bin/bash
read -p 'Give me group ' group
for var in $(ps -G $group -o pid)
	do
	if (echo "$var" | grep -E -q "^?[0-9]+$")
		then
		if [ -d /proc/$var/ ]
			then
			IFS=$'\t'
			result+=( $(cat -v /proc/$var/cmdline) )
		fi 
	fi
done

if [ "${#result[@]}" -ne "0" ]
	then
	sorted=($(sort <<<"${result[*]}"))
fi

for i in ${result[@]}
	do
	echo -e "$i \n"
done
