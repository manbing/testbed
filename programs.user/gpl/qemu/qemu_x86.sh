#! /bin/bash
echo [$0][$1]

case $1 in
start)
	sudo qemu-system-x86_64 -smp 2 -m 2G -kernel ./image/bzImage -initrd ./image/rootfs.img -append "root=/dev/ram rdinit=/sbin/init console=ttyS0" -nographic -net nic -net tap,ifname=tap0,script=no
;;

*)
	echo "[start]"
;;
esac
