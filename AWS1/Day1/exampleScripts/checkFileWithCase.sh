#!/bin/bash
if [[ "$#" -eq 0 ]]; 
	then 
		echo "no parameters were passed to this script!"
		echo -e "usage: $(basename $0) <arg1> <arg2> ...<argn>"
		exit 1
fi
filename="$1"
check=$(file $1 | cut -d " " -f2)
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