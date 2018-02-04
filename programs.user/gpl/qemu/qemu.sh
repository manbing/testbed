#! /bin/bash
echo [$0][$1]


case $1 in 
start)
	qemu-system-arm -M stm32-p103 -nographic -kernel ./arm-image/kernel/zImage -initrd arm-image/rootfs.img -append "root=/dev/ram rdinit=/bin/sh kgdboc=ttyAMA0,115200 kgdbwait"

# it is okay command
#qemu-system-arm -M vexpress-a9 -cpu cortex-a9  -m 128M -kernel ./arm-image/kernel/zImage -initrd arm-image/rootfs.img -dtb arm-image/kernel/vexpress-v2p-ca9.dtb -append "root=/dev/ram rdinit=/bin/sh kgdboc=ttyAMA0,115200 kgdbwait"
;;

host_bridge)
	ip address del 192.168.110.132/24 dev ens33
	ip link add name br0 type bridge
	ip link set br0 up
	ip address add 192.168.110.132/24 dev br0
	ip link set ens33 up
	ip link set ens33 master br0
	ip route add  default via 192.168.110.2 dev br0
;;

qemu_bridge)
	ip link set tap0 up
	ip link set tap0 master br0
;;

*)
	echo "[start|host_bridge|qemu_bridge]"
;;
esac
