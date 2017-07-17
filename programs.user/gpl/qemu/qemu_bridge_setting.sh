#!/bin/bash

echo "Configure ens33 IP as 0.0.0.0"
ip address del 192.168.110.131/24 dev br0

echo "Create and set up bridge"
ip link add name br0 type bridge
ip link set br0 up
ip address add 192.168.110.131/24 dev br0

echo "Add ens33 adn tap0 into bridge"
ip link set tap0 up
ip link set ens33 up
ip link set ens33 master br0
ip link set tap0 master br0
