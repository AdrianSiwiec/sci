#!/bin/bash 

unitTestDir="obj/test"

for test in "$unitTestDir"/*.e; do 
        if [ $(basename $test) != "main.e" ]; then
		echo "Running $(basename $test)" && \
		./$test || exit;
        fi 
	done &&	echo "ALL OK"