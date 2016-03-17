#!/bin/bash

mem=$1
shift
if [ $mem -gt 124000 ] && [ $mem -lt 1000000000 ]; then
 for i in net.core.rmem_max net.core.wmem_max net.core.wmem_default net.core.rmem_default; do
   sysctl -w $i=$mem
 done
else
 for i in net.core.rmem_max net.core.wmem_max net.core.wmem_default net.core.rmem_default; do
   sysctl $i
 done
 echo 'be careful'
fi
