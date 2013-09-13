#!/bin/sh
T_PRJDIR=$(pwd | xargs dirname)
path_list="
    $T_PRJDIR
    "
file_list="
    "
remove_list="
    "
# output filename
src_filelist=src_filelist
lookup_filelist=lookup_filelist
ctags_output=tagsdebug
cscope_output=cscope
lookupfile_tags=filenametags
 
stdIncInput=~/work/env/normal/devInputStdInc.sh
if [ -f $stdIncInput ]; then
    . $stdIncInput
else
    echo "file $stdIncInput is not exist"
    exit
fi

