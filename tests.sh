#!/bin/bash
killall was.o;make clean;make;sudo ./was.o


#Success combination

COUNTER=0
while [  $COUNTER -lt 100 ]; do
    let COUNTER=COUNTER+1
    echo "Trying number $COUNTER"
    echo "" > /dev/tcp/localhost/43657;echo "" > /dev/tcp/localhost/55981
done

#Other port than known ports to service

COUNTER=0
while [  $COUNTER -lt 100 ]; do
    let COUNTER=COUNTER+1
    echo "Trying number $COUNTER"
    echo "" > /dev/tcp/localhost/23657
done

#Only first port

COUNTER=0
while [  $COUNTER -lt 100 ]; do
    let COUNTER=COUNTER+1
    echo "Trying number $COUNTER"
    echo "" > /dev/tcp/localhost/43657
done

#Only second port

COUNTER=0
while [  $COUNTER -lt 100 ]; do
    let COUNTER=COUNTER+1
    echo "Trying number $COUNTER"
    echo "" > /dev/tcp/localhost/55981
done
