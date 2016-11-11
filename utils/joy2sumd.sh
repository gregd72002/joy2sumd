#!/bin/sh
while [ 1 ]; do
    sleep 1;
    if [ -c $1 ] && [ -c $2 ]; then 
	stdbuf -oL /usr/local/bin/joy2sumd -a $1 -b $2 >> /tmp/joy2sumd.log 2>&1
    fi
done

