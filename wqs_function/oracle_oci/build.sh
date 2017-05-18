#!/bin/bash
oci_path=`pwd`
oci_path="$oci_path/oci"
echo $oci_path
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$oci_path

cc oracle_oci.c -I$oci_path/sdk/include -L$oci_path -lclntsh
