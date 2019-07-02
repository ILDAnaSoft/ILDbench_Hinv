#!/bin/bash

for val in mkfmrec mrec
do
    for pol in -1 +1
    do
	root -l -b -q Easy_UL.C+\(${pol},\"${val}\"\) 2>&1 | tee output/UL900_${val}${pol}.txt
    done
done
