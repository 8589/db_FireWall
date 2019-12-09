#!/bin/bash
main_src=`pwd`/..
#echo $main_src
#echo $main_src

sub_src=(/utils/CJsonObject/build /utils/base_sql_parser/build /build)

for suffix in ${sub_src[*]}
do

	#echo "$main_src$suffix"
	mkdir $main_src$suffix
	cd $main_src$suffix
	echo `pwd`
	cmake ..
       	make
done
