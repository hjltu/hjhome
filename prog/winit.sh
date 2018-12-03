#!/bin/bash

NEWNAME=$2
SSID=$3
PASS=$4
MQTT=$5

if [ "$#" -ne 5 ];then
	echo "wrong args count"
	exit 1
fi

NAME="wemos"
SRV=$1
NEWNAME=$2
SSID=$3
PASS=$4
MQTT=$5

echo "first you should to subscribe to:"
echo "mosquitto_sub -h $SRV -t /$NAME/out/# -v"
echo ''
echo "current name: $NAME"
echo "current server: $SRV"
echo "name: $NEWNAME"
echo "ssid: $SSID"
echo "pass: $PASS"
echo "mqttsrv: $MQTT"
read -p " is it correct? y/n :" ANS
if [[ $ANS != y ]];then
	echo "answer: $ANS exit"
	exit 1
fi

#exit 0

sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/period -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/http -m 1
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/blinds -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/shutters -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/term -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/count -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/gap -m 9
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/sensor -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/value0 -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/value1 -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/value2 -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/value3 -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/pwm12 -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/pwm13 -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/rele14 -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/rele16 -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/somfy11 -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/buttons -m 0
sleep .2
mosquitto_pub -h $SRV -t /$NAME/in/state -m 1
sleep .2

echo "current default"
mosquitto_pub -h $SRV -t /$NAME/in/default -m qq
mosquitto_pub -h $SRV -t /$NAME/in/name -m $NEWNAME
mosquitto_pub -h $SRV -t /$NAME/in/ssid -m $SSID
mosquitto_pub -h $SRV -t /$NAME/in/pass -m $PASS
mosquitto_pub -h $SRV -t /$NAME/in/mqtt -m $MQTT

sleep 2
echo "reset"
mosquitto_pub -h $SRV -t /$NAME/in/reset -m qq
sleep 5
echo "new default"
mosquitto_pub -h $SRV -t /$NAME/in/default -m qq

read -p " !!! set default to 0 !!! y/n :" ANS
if [[ $ANS != y ]];then
	echo "answer: $ANS exit"
	mosquitto_pub -h $SRV -t /$NAME/in/default -m qq
	exit 1
fi
echo "default=0"
mosquitto_pub -h $SRV -t /$NAME/in/default -m 0
























