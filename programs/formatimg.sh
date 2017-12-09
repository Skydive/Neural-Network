#!/bin/bash
name=$(basename -s .png $1)
echo $(convert $1 txt:- | tail -n +2 | awk '{print $2}' | awk -F, '{print substr($1, 2)/65535;}' | tr '\r\n' ' ') > $name.input
