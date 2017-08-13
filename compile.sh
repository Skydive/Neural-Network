# KMake - because a Makefile with small projects is for losers!
#!/bin/bash
rm main

CPP_FILES="./src/Main.cpp"
files=$(find src -name '*.hpp')
pattern="#pragma implement "
for f in $files; do
	while read -r line; do
		if [[ $line =~ $pattern ]]; then
			line=$(echo $line | cut -d " " -f3-)
			for arg in $line; do
				CPP_FILES="$CPP_FILES $(dirname $f)/$arg.cpp"
			done
		fi
	done < $f
done
g++ $CPP_FILES -o main \
	-std=c++1z -Iinclude \
	$(pkg-config eigen3 zlib --cflags --libs) \
	-msse2 -g -pipe \
	-ldl -Wl,--rpath=./
