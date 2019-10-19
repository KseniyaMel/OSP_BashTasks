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
	touch result.xls
	echo -e "Name \t Arguments" >> result.xls
fi

for i in ${result[@]}
	do
	echo -e "$i \n"
done

# for var in ${result[@]}
# do
# count=$(grep -o "/" <<< $var| wc -l)
# if [ "$count" -gt "2" ]
# then
# name=${var%% *}
# arg=${var#* }
# echo -e "$name \t $arg" >> result.xls
# else
# echo -e "$var \t 0" >> result.xls
# fi
# done

