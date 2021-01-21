#!/bin/bash

end=$((SECONDS+21600))

while [ $SECONDS -lt $end ]; do
	xdotool click 1
	sleep "1.$RANDOM"
done

