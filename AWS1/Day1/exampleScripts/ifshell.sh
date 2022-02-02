#!/bin/bash
# check if the number of arguments passed is 0
if [[ "$#" -eq 0 ]]; 
	then 
		echo "no parameters were passed to this script!"
		echo -e "usage: $(basename $0) <arg1> <arg2> ...<argn>"
		exit 1
else
	echo "you have entered following arguments $@"
fi