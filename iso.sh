#!/bin/sh
set -e
. ./build.sh
 
mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub
 
cp sysroot/boot/SimpleOS2.kernel isodir/boot/SimpleOS2.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "SimpleOS2" {
	multiboot2 /boot/SimpleOS2.kernel
	boot
}
EOF
grub-mkrescue -o SimpleOS2.iso isodir
echo "Complete"