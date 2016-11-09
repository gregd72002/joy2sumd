#!/bin/sh
while [ 1 ]; do
    sleep 1;
    if [ -c /dev/input/js0 ] && [ -c /dev/ttyAMA0 ] && [ -c /dev/rfcomm0 ] && [ -c /dev/ttyACM0 ]; then 
	/usr/local/bin/joy2sumd -a /dev/input/js0 -b /dev/ttyAMA0 -c /dev/rfcomm0 -d /dev/ttyACM0 
    fi
done

