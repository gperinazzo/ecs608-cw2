#!/bin/bash

for _i in `seq $4`
do
    ./cw2 $1 $2 $3
	sleep 1 # Wait at least 1 sec (time is the random seed)
done
