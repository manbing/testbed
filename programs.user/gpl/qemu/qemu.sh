#! /bin/bash
echo [$0][$1]


case $1 in 
"start" )
	qemu-system-arm -M vexpress-a9 -cpu cortex-a9 -m 1024M -net nic -net tap -kernel ./arm-image/kernel/zImage -dtb arm-image/kernel/vexpress-v2p-ca9.dtb -initrd arm-image/rootfs.img -serial stdio -append " root=/dev/ram rdinit=/sbin/init"
;;

"host_bridge" )
	ip address del 192.168.110.132/24 dev ens33
	ip link add name br0 type bridge
	ip link set br0 up
	ip address add 192.168.110.132/24 dev br0
	ip link set ens33 up
	ip link set ens33 master br0
	ip route add  default via 192.168.110.2 dev br0
;;

"qemu_bridge" )
	ip link set tap0 up
	ip link set tap0 master br0
;;

?)
	echo "[start|host_bridge|qemu_bridge]"
;;
esac
