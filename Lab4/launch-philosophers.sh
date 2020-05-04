#! /bin/bash


if [ $# -ne 1 ] ; then 
   echo 
   echo "Usage: $0 seats s "
   echo "where seats is the setting capacity of the table"
   echo ""
   exit 1
else
	count=1
	while [ ${count} -le $1 ] ; do
      ./dining-p $1 ${count} &      
      echo "PID of '${dining-p} $1 ${count}' is " $!
      count=$(( $count + 1 ))
	done   
fi



