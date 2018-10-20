#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Uso: prueba hostname puerto"
fi

hostname=$1
puerto=$2

sensores=(temperatura_ambiente temperatura_objeto presion humedad luminosidad sensacion_termica)

for sensor in "${sensores[@]}"
do
	./cliente $sensor $hostname $puerto
	echo
done