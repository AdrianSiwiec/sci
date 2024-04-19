#!/bin/bash 

unitTestDir="obj/test"

for test in "$unitTestDir"/*test.e; do 
        if [ $(basename $test) != "main.e" ]; then
		echo "Running $(basename $test)" && \
		./$test || exit;
        fi 
	done &&	echo "ALL OK"