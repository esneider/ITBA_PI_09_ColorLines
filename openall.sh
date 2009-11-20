#!/bin/bash

extensions=".h .c"
nothaving="test"

files=""
nothaving=`echo $nothaving | sed "s# #|#g"`
for i in $extensions
do
	for j in `find ./ -name "*$i" | egrep -v "$nothaving"`
	do
		files="$files $j"
	done
done

aux=""
for i in $files
do
	aux="$aux  `echo $i | egrep -o "/[^/]*$"`$i"
done

files=`echo "$aux" | sed "s# #\n#g" | sort | egrep -o "./[[:print:]]*$"`

kate $files &
# agregarle la opcion -u para que agregue a una ventana de kate ya existente
