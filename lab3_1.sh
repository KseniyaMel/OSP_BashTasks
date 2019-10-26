#!/bin/bash
print_all_users()
{
        cat /etc/passwd | awk -F: '{print $1}'
}
print_group_users()
{
        cat /etc/passwd | grep $1 | awk -F: '{print $1}'
}
print_user()
{
        ls -l $1 | awk '{print $3;}'
}


file_based=`ls -ln $1`
acces_mode=`echo $file_based | awk '{print $1;}'`
uid=`echo $file_based | awk '{print $3;}'`
gid=`echo $file_based | awk '{print $4;}'`

user_possibility=${acces_mode:1:1}
group_possibility=${acces_mode:4:1}
all_possibility=${acces_mode:7:1}

if [[ "$all_possibility" ==  "r" ]]; then
        print_all_users
elif [[ "$group_possibility" == "r" ]]; then
        print_group_users $gid
elif [[ "$user_possibility" == "r" ]]; then
        print_user $1
fi
