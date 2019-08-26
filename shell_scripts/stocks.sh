#!/bin/bash

stock=$1

Main () {
     if [ "$stock" = "" ];
    then
    	echo "-1 -1 -1" #error
    else
	paragraph=$(w3m "https://finance.yahoo.com/quote/$stock.SA" |grep BRL -A 2)
	echo $paragraph | sed 's/.*BRL//'  | sed 's/[^-.0-9 ]*//g'
     fi
}

Main

#echo baixa -0.08 -2.50 3.12
