#!/bin/bash

cheat=${1:-OZBI}

echo ln -nfs ../output/cheat${cheat}/root_merge data_cheat
ln -nfs ../output/cheat${cheat}/root_merge data_cheat
ls -l data_cheat
