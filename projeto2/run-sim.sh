#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 sim_config sim_data"
    exit 1
fi

simconfig=$1
simdata=$2

pids=""

while read line
do
    ./demo_carro $(<$simconfig) <<< "$line" >> results.out &
    pids="$pids $!"
done < $simdata

wait $pids
