#! /bin/sh

echo "Configure ens33 IP as 0.0.0.0"
ip address del 192.168.110.132/24 dev ens33

echo "Create and set up bridge"
ip link add name br0 type bridge
ip link set br0 up
ip address add 192.168.110.132/24 dev br0
ip link set ens33 up
ip link set ens33 master br0
ip route add  default via 192.168.110.2 dev br0

echo "Add ens33 adn tap0 into bridge"
ip link set tap0 up
ip link set tap0 master br0
#ip address add 192.168.110.87/24 dev eth0
#ip route add  default via 192.168.110.2 dev eth0
