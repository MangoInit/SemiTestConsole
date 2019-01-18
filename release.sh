#!/bin/sh

exe="TestConsole"
des="/home/mango/myQT/TestConsole1.1/ldd"

deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')

cp $deplist $des
