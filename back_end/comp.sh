#!/bin/bash

headers=( error game colors mytime commands)

comp_options="-Wall -pedantic -std=c99 -O0 -lm"

libpath="back_end/lib/"

includepath="back_end/include/"

#################################################################

paths=""

function libcompile {
    touch $libpath$1.a $libpath$1.o
    rm $libpath$1.a $libpath$1.o
    gcc $comp_options -I$includepath -L$libpath -c $libpath$1.c
    mv $1.o $libpath
    ar r $libpath$1.a $libpath$1.o
    ranlib $libpath$1.a
   # rm $libpath$1.o
    
    paths=$paths" $libpath$1.o"
}

function sourcecompile {
    echo -e "\e[94mNow compiling\e[0m \e[4m$i\e[0m...\n"
    gcc $comp_options -I$includepath -L$libpath $i $paths
}

echo -e "\n\e[94mCompiling...\e[0m\n"

for i in "${headers[@]}"
do
    libcompile $i
done

echo -e "\e[94mOK\e[0m\n"

echo -e "paths = $paths\n"

for i in "$@"
do
    sourcecompile $i
done