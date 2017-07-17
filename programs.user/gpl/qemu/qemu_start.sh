#!/bin/bash

qemu-system-arm -M vexpress-a9 -cpu cortex-a9 -m 1024M -net nic -net tap -kernel ./arm-image/kernel/zImage -dtb arm-image/kernel/vexpress-v2p-ca9.dtb -initrd arm-image/rootfs.img -serial stdio -append " root=/dev/ram rdinit=/sbin/init"
