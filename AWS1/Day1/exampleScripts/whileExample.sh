#!/bin/bash
countDown=10
while [ $countDown -gt 0 ] ; do
	echo $countDown
	let countDown=$countDown-1
done