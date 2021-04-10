#!/bin/bash

if [ "$(id -u)" != "0" ]; then
   echo "    Install MUST be run as root" 1>&2
   exit 1
fi

# Update and install needed packages - server and service
apt-get update
apt-get upgrade -y
apt-get install mosquitto -y
apt-get install nodered -y

# Run services at boot
systemctl enable mosquitto.service
systemctl enable nodered.service
