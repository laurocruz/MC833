#!/bin/sh

declare -a sites=("www.ic.unicamp.br" "www.unicamp.br" "www.usp.br" "www.unesp.br" "www.cs.man.ac.uk" "www.u-tokyo.ac.jp")

echo "------------------------------------------------------ A)"
for i in "${sites[@]}"
do
    ping $i -c 10
    printf "\n\n"
done

echo "------------------------------------------------------ B)"
ping  www.lrc.ic.unicamp.br -c 5

echo "------------------------------------------------------ C)"

netdev=$(ifconfig | grep UP,BROADCAST,RUNNING,MULTICAST | awk '{print $1}')
netdev=${netdev::-1}

loopdev=$(ifconfig | grep LOOPBACK | awk '{print $1}')
loopdev=${loopdev::-1}


printf "Local IP: "
ifconfig $netdev | grep "inet " | awk '{print $2}'
printf "\n"

printf "Public IP: "
curl ipinfo.io/ip
printf "\n"

printf "Interface de rede: $netdev\n"

printf "Interface de loopback: $loopdev"
printf "\n\n"

echo "------------------------------------------------------ D)"
printf "Default Gateway: "
route -n | grep UG | awk '{print $2}'
printf "\n\n"

echo "------------------------------------------------------ E)"
printf "DNS Server: "
nmcli dev show | grep DNS | awk '{print $2}'

printf "\n"

for i in "${sites[@]}"
do
    printf "$i: "
    nslookup $i | grep Address | awk '{l++}l==2' | awk '{print $2}'
    printf "\n"
done

echo "------------------------------------------------------ F)"

for i in "${sites[@]}"
do
    traceroute $i
    printf "\n\n"
done

echo "------------------------------------------------------ G)"

netstat | grep tcp

echo "------------------------------------------------------ H)"

telnet www.ic.unicamp.br 80
GET /~reltech/
host: www.ic.unicamp.br
