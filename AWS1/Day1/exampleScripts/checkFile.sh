#!/bin/bash
# check if the number of arguments passed is 0
if [[ "$#" -eq 0 ]]; 
	then 
		echo "no parameters were passed to this script!"
		echo -e "usage: $(basename $0) <arg1> <arg2> ...<argn>"
		exit 1
fi

filename="$1"
echo "$filename"
# -e is  checking existance of a file
if [ -e "$filename" ]; then
	echo "file exixts"
else
	echo "file doesn't exist"
fi
