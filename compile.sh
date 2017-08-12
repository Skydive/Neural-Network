#!/bin/bash
rm main

CPP_FILES="Main.cpp"
files=$(find . -name '*.hpp' -not -path './include/*')
pattern="#pragma implement "
for f in $files; do
	while read -r line; do
		if [[ $line =~ $pattern ]]; then
			line=$(echo $line | cut -d " " -f3-)
			for arg in $line; do
				CPP_FILES="$CPP_FILES $arg.cpp"
			done
		fi
	done < $f
done

g++ $CPP_FILES -o main \
	-std=c++1z -Iinclude \
	$(pkg-config sdl2 eigen3 zlib --cflags --libs) \
	-lpthread -msse2 -g -pipe
