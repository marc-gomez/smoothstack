#!/bin/bash
# check if the number of arguments passed is 0
if [[ "$#" -eq 0 ]]; 
	then 
		echo "no parameters were passed to this script!"
		echo -e "usage: $(basename $0) <arg1> <arg2> ...<argn>"
		exit 1
fi

for i in $@ ; do
	#Below command will run file and extract the second pice of output and put it in check variable
	#cut will tokenize the string using the " " which we specified	
	filename="$i"
	check=$(file $i | cut -d " " -f2)
	case $check in
		"ASCII")
			echo -e "\n File $filename is an ASCII file"
		;;
		"Bourne-Again")
			echo -e "\n File $filename is a script file"
		;;
		"ELF")
			echo -e "\n File $filename is an EXECUTABLE file"
		;;
		*)
			echo -e "\n can not determine type of $filename"
		;;
	esac
done