#!/bin/bash

qemu-system-arm -M vexpress-a9 -cpu cortex-a9 -m 1024M -net nic -net tap -kernel ./normal-arm-linux/kernel/zImage -dtb ./normal-arm-linux/kernel/vexpress-v2p-ca9.dtb -initrd ./normal-arm-linux/rootfs.img -serial stdio -append " root=/dev/ram rdinit=/sbin/init"
