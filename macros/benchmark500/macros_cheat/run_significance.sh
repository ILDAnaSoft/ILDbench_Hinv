#!/bin/bash

detector=${1:-l5}
cheat=${2:-cheatOZBI}
cut=7

for pol in -1 +1
do
    root -l -b -q signf_easy.C\(${pol},\"${detector}\",\"${cheat}\"\) 2>&1 \
	| tee output_${detector}/${cheat}/sig${pol}_cut${cut}_` date "+%Y%m%d"`.txt
done
