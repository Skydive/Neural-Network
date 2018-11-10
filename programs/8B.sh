#!/bin/bash
cd "${0%/*}"

if [ -z "$2" ] && [ -z "$3" ]; then
	read -p "Enter 8-bit number: " in1
	read -p "Enter 8-bit number: " in2
else
	in1="$2"
	in2="$3"
fi
in1=$(echo $in1 | sed -e 's/\(.\)/\1 /g')
in2=$(echo $in2 | sed -e 's/\(.\)/\1 /g')
in="$in1 $in2"
cmd="../bin/main -read \"../programs/8B-$1.zlib\" -feedforward \"$in\" -output | tail -n1"
output=$(eval $cmd)
output=$(echo $output | sed -e 's/ //g')
echo $output
