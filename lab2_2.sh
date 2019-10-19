#!/bin/bash
check1()
{
	flag1=0
	flag2=0
	way=$1
	for var in $1/*
		do
		if [ -d $var ]
			then
			let flag1=1
		elif [ -f $var ]
			then
			let flag2=1
		fi
	done
	if [ "$flag1" -eq "0" ] && [ "$flag2" -eq "1" ]
		then
		name=${way##*/}
		result+=( $name )
	fi
}
IFS=$'\t'
read -p 'Give me way ' way
if [ -d $way ]
	then
	result=()
	for var in $way/*
		do
		if [ -d $var ]
			then
			check1 $var fi
	done
if [ "${#result[@]}" -ne "0" ]
	then
	sorted=($(sort <<<"${result[*]}"))
	echo ${result[@]}
	else
	echo "Нет подходящего каталога"
fi
else
echo "It isn't way"
fi

