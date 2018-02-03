#!/bin/bash

cont=$1
value=0
while [ $cont != 0 ]
do
xterm -geometry 20x20+$value+0 -e ./execute.sh &

value=$[value+200]
cont=$[cont-1]
done