#!/bin/bash

comp_options="-Wall -pedantic -std=c99 -O0 -lm"

libpath=( "back_end/lib/" "front_end/lib" )

includepath=( "back_end/include/" "front_end/include" )

#################################################################


function sourcecompile {
    echo -e "\e[94mNow compiling\e[0m \e[4m$i\e[0m...\n"
    direc = ""
    for i in "${libpath[@]}"
    do
    	direc="$direc -I$i"
    done
    for i in "${includepath[@]}"
    do
    	direc="$direc -L$i"
    done
    gcc $comp_options $direc $i
}

echo -e "\n\e[94mCompiling...\e[0m\n"

back_end/comp.sh
front_end/comp.sh

echo -e "\e[94mOK\e[0m\n"

for i in "$@"
do
    sourcecompile $i
done
