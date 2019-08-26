#!/bin/bash

function Volume() {
    vol=$(amixer get Master |grep Left |awk '{print $5}'|sed 's/[^0-9\%]//g')

    if [ "$vol" == "" ]
    then
    	vol=$(amixer get Master |grep "Mono: Playback" |awk '{print $4}'|sed 's/[^0-9\%]//g')       
    fi
    echo $vol
    # vol=$(osascript -e 'get volume settings' | grep "output volume" | awk '{print $2}' | sed 's/[^0-9]//g')
    # echo $vol
}

Volume
