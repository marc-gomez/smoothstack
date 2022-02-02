#!/bin/bash
read -p "enter you first number: " value1
read -p "enter you second number: " value2

let answer=$value1*$value2
echo "$answer"