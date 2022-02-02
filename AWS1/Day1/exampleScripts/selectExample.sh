#!/bin/bash
options="pizza tacos"
#PS3 is a prompt when system runs our select
PS3="Enter Choice"
select choice in $options ; do
	echo "reply variable has value $REPLY in it"
	echo "choice variable has value $choice in it"
	break
done
