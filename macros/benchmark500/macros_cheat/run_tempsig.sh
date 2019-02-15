#!/bin/bash

detector=${1:-l5}

for pol in -1 +1
do
    root -l -b -q signf_temp_new.C\(${pol},\"${detector}\"\) 2>&1 \
	| tee output_${detector}/cheatTemp/sig${pol}_` date "+%Y%m%d"`.txt
done
