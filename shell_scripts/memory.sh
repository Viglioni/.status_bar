#!/bin/bash

# Memory
free=$(free -m|grep Mem)
words=(${free})
m_total=${words[1]}
m_used=${words[2]}

#Swap
free=$(free -m|grep Swap)
words=(${free})
s_total=${words[1]}
s_used=${words[2]}

# return
printf  "RAM: $m_used/$m_total, Swap: $s_used/$s_total"

