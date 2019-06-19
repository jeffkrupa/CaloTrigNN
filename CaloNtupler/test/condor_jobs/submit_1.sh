#!/bin/bash

rm log/*
rm output/*
rm error/*

source tar.sh
NN=$1
bash submit_2.sh submit_3.sh $PU $x
