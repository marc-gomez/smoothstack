#!/bin/bash
educate_user()
{
	echo "Salad is a better choice"
}
options="pizza tacos salad"
echo "choose a food"
#PS3 is a prompt when system runs our select
PS3="Enter Choice"
select choice in $options ; do
	if [ "$choice" = 'salad' ] ; then
		echo "good choice"
		exit 0
	else
		educate_user
	fi
	
done
