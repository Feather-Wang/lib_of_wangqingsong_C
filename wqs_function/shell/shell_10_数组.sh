#!/bin/bash 

index=0
arr[index]="str1"
index=1
arr[index]="str2"
arr[2]="str3"

for arg in ${arr[@]}
do
    echo "arg --> $arg"
done
