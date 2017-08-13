# KMake - because a Makefile with small projects is for losers!
#!/bin/bash


if [ -z "$1" ]; then
	GENERATORS=$(find src/generator -name '*.cpp')
else
	GENERATORS="src/generator/$1.cpp"
fi

for g in $GENERATORS; do
	CPP_FILES="$g"
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
	g++ $CPP_FILES -fPIC -shared -o $(basename -s .cpp $g).so \
		-std=c++1z -Iinclude \
		$(pkg-config eigen3 zlib --cflags --libs) \
		-ldl -msse2 -g -pipe
	if [ $? -eq 0 ]; then
		echo Created $(basename -s .cpp $g).so
	fi
done
