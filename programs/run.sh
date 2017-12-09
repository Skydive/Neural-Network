#!/bin/bash
./formatimg.sh number.png
num=$(cat number.input)
eval "../main -read programs/MNIST30.zlib -feedforward \"$num\" -outputfancy"
