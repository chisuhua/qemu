#!/bin/bash
# -*- coding: utf-8 -*-

./x86_64-softmmu/qemu-system-x86_64 -hda ./kmddev_ubuntu.img \
	--enable-kvm -smp cores=2,threads=1 -m 4096 \
	-no-acpi -no-hpet -k en-us \
	-device qxl-vga,id=video0,ram_size=67108864,vram_size=67108864,vgamem_mb=16 \
	-spice port=5920,addr=0.0.0.0,disable-ticketing,seamless-migration=on \
	-fsdev local,security_model=none,id=fsdev0,path=/tmp/share -device virtio-9p-pci,id=fs0,fsdev=fsdev0,mount_tag=hostshare \
	-usbdevice tablet \
	-monitor stdio

#	-device isa-serial,chardev=charserial0,id=serial0 -chardev pty,id=charserial0 -device 

# below is boot from cdrom and install ubuntu
#./x86_64-softmmu/qemu-system-x86_64 -hda ./kmddev_ubuntu.img \
#	 -cdrom ~/Downloads/ubuntu-16.04.4-desktop-amd64.iso -boot d \
#	--enable-kvm -smp cores=2,threads=1 -m 4096 \
#	-no-acpi -no-hpet -k en-us \
#	-device qxl-vga,id=video0,ram_size=67108864,vram_size=67108864,vgamem_mb=16 \
#	-spice port=5920,addr=0.0.0.0,disable-ticketing,seamless-migration=on \
#	-fsdev local,security_model=none,id=fsdev0,path=/tmp/share -device virtio-9p-pci,id=fs0,fsdev=fsdev0,mount_tag=hostshare \
#	-usbdevice tablet \
#	-monitor stdio


