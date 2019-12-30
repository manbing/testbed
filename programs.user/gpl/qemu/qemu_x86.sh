#! /bin/bash
echo [$0][$1]

case $1 in
start)
	sudo qemu-system-x86_64 -smp 2 -m 1G -kernel ./image/bzImage -initrd ./image/rootfs.img -append "root=/dev/ram rdinit=/sbin/init console=ttyS0" -nographic -net nic -net tap,ifname=tap0,script=no
;;

kgdb)
	sudo qemu-system-x86_64 -smp 2 -m 1G -kernel ./image/bzImage -initrd ./image/rootfs.img -serial tcp::1234,server,nowait -append "root=/dev/ram rdinit=/sbin/init kgdbwait kgdboc=ttyS0,115200" -net nic -net tap,ifname=tap0,script=no
;;

gdb)
	sudo qemu-system-x86_64 -smp 2 -m 1G -kernel ./image/bzImage -initrd ./image/rootfs.img -serial stdio -append "root=/dev/ram rdinit=/sbin/init" -S -gdb tcp::1234
;;

*)
	echo "[start]"
;;
esac
