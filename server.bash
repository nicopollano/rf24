#!/bin/bash

LightRoom="R00ML16T"
LightOff="L1G4T0OF"
LightOn="L1G4T0N"
UnlockD0="UnL0C4D0"
LockD0="L0C4D0"

sudo killall nc
sudo killall ncat

TEMPFILE=$(mktemp)

ncat  -k -l  4557  > nc_out &

while true; do
    while read -r line; do
        echo "In: $line"
        case $line in
            $LightRoom)
                echo -id=0 -alternate-light= >> /tmp/clockserverfifo
            ;;
            $LightOff)
                echo -id=0 -turn-on-light=0 >> /tmp/clockserverfifo
            ;;
            $LightOn)
                echo -id=0 -turn-on-light=1 >> /tmp/clockserverfifo
            ;;
            $UnlockD0)
                echo -id=0 -lock-device=0 >> /tmp/clockserverfifo
            ;;
            $LockD0)
                echo -id=0 -lock-device=1 >> /tmp/clockserverfifo
            ;;
            *)
                echo "Unknown command"
            ;;
        esac

      
        
    done < nc_out
    echo "Disconnected"
done 
