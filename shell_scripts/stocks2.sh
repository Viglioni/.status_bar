#!/bin/bash

stock=$1

function Main(){
     if [ "$stock" == "" ];
    then
    	echo "error"
    else
	paragraph=$(w3m "http://cotacoes.economia.uol.com.br/acao/index.html?codigo=$stock.SA" |grep Horário -A 2)
	paragraph=$(echo $paragraph | tr , .)
	word=( ${paragraph} )
	
	status=${word[9]}
	var=${word[11]}
	if [ "$var" == "0.00" ]
	then
	    status="estável"
	fi
	perc=${word[12]}
	atual=${word[13]}
	echo $status $var $perc $atual
     fi
}

Main

#echo baixa -0.08 -2.50 3.12
