#!/bin/bash

cheat=${1:-cheatOZBI}
cut=7

for pol in -1 +1
do
    root -l -b -q signf_easy.C\(${pol},\"${cheat}\"\) 2>&1 \
	| tee output/${cheat}/sig${pol}_cut${cut}_` date "+%Y%m%d"`.txt
done
