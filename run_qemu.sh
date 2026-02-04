#!/bin/bash
QEMU=qemu-system-arm
KERNEL=~/Desktop/linuxsystem/qemu-buildroot/buildroot-2024.02.3/output/images/zImage
DTB=~/Desktop/linuxsystem/qemu-buildroot/buildroot-2024.02.3/output/images/vexpress-v2p-ca9.dtb
ROOTFS=~/Desktop/linuxsystem/qemu-buildroot/buildroot-2024.02.3/output/images/rootfs.ext2

${QEMU} \
    -M vexpress-a9 \
    -m 1024M \
    -smp 2 \
    -kernel ${KERNEL} \
    -dtb ${DTB} \
    -append "console=ttyAMA0,115200 root=/dev/mmcblk0 rw init=/sbin/init" \
    -drive file=${ROOTFS},if=sd,format=raw \
    -serial stdio \
    -net nic,model=lan9118 \
    -net user,hostfwd=tcp::2222-:22,hostfwd=tcp::8080-:80 \
    -no-reboot
