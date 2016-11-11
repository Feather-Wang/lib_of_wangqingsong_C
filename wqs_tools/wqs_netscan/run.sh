#!/bin/bash
if [ $# != 4 ]
then
	echo "example : ./run.sh eth0 192.168.25 1 254"
	exit
fi

for((i = $3; i <= $4; i++))
do
	{
		info=`sudo ./a.out $1 $2.$i`
		if [ "$info" != "" ]
		then
			echo -e "$info"
		fi
	}&
done

wait
