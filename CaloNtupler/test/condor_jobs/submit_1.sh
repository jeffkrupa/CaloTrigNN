#!/bin/bash

PU=$1
START=$2
END=$3

for x in `seq $2 $3`; do 
	bash submit_2.sh submit_3.sh $PU $x
done 
