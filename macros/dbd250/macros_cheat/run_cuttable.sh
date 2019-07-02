#!/bin/bash

for pol in -1 +1
do
    root -l -b -q cuts_qqxinv.C+\(${pol}\) 2>&1 | tee output/cuttable${pol}_` date "+%Y%m%d"`.txt
done
