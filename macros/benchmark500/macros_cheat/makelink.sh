#!/bin/bash

ls=${1:-l5}
cheat=${2:-OZBI}

for dir in output pdf
do
    echo ln -nfs ${dir}_$ls $dir
    ln -nfs ${dir}_$ls $dir
    ls -l $dir
done

PathToOutput=../../../scripts

echo ln -nfs $PathToOutput/output/full/$ls/root_merge data_final
ln -nfs $PathToOutput/output/full/$ls/root_merge data_final
ls -l data_final

echo ln -nfs $PathToOutput/output/cheat$cheat/$ls/root_merge data_cheat
ln -nfs $PathToOutput/output/cheat$cheat/$ls/root_merge data_cheat
ls -l data_cheat
