#!/bin/bash
countDown=10
until [ $countDown -le 0 ] ; do
	echo $countDown
	let countDown=$countDown-1
done