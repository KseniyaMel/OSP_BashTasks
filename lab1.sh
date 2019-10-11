#!/bin/bash

# вывод ошибок в файл
error_exit()
{
"$1" 2>> $HOME/lab_err.txt
}

four()
{
	echo
	echo "Напишите имя файла"
	read name
	if [ -f $name ]
		then
		result=$(cat $name)
		echo $result
		else
		error_exit "cat $name"
		echo "Такого файла не существует"
	fi
}

# функция для удаления файла
five()
{
	echo
	echo "Напишите имя файла"
	read name
	if [ -f $name ]
		then
		echo -e "Удалить файл $name?(д/н)"
		read answer
		if [ "$answer" = "д" ]
			then
			if [ "$name" = "$HOME/lab_err.txt" ]
				then 
				rm $name
				touch $HOME/lab_err.txt 
			fi
			echo -e "Файл удален"
		elif [ "$answer" = "н" ]
			then
			return
		else
			echo "Некоректный ответ"
			five
		fi
	else
		error_exit rm $name
		echo "Такого файла не существует"
		return
	fi
}

six()
{
clear
exit
}


while [ 1=1 ] 
do
# каждый раз очищаем экран, выводим инструкцию и считываем номер действия 
clear
echo -e  "1.Напечатать имя текущего каталога \n2.Напечатать содержимое текущего каталога \n3.Вывести текущую дату и время в формате Wed Feb 2 01:01 MSK 2015 \n4.Вывести содержимое файла на экран \n5.Удалить файл \n6.Выйти из программы"
read number 
IFS=$'\n'

if [[ $((number)) != $number ]] # проверяем число ли это
then
echo -e "Некорректная команда \n" 
read -n 1

elif [ "$number" -eq "1" ]
then
echo
pwd
read -n 1

elif [ "$number" -eq "2" ]
then
echo
ls
read -n 1

elif [ "$number" -eq "3" ]
then
echo
date
read -n 1

elif [ "$number" -eq "4" ]
then
four
read -n 1

elif [ "$number" -eq "5" ]
then
five
read -n 1

elif [ "$number" -eq "6" ]
then
six

fi
done