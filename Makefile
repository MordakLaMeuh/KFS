IMG_DISK = image_disk.img
IMAGE_SIZE = 9316
LOOP_DEVICE = $(shell sudo losetup -f)

all: $(IMG_DISK)
	make -C nucleus DEBUG=$(DEBUG) OPTIM=$(OPTIM)
	sudo losetup -fP $(IMG_DISK)
	sudo mount $(LOOP_DEVICE)p1 /mnt
	sudo cp -vf nucleus/build/kernel.elf /mnt
	sudo umount /mnt
	sudo losetup -d $(LOOP_DEVICE)

$(IMG_DISK):
	dd if=/dev/zero of=$(IMG_DISK) bs=1024 count=$(IMAGE_SIZE)
	( echo -e "o\nn\np\n1\n2048\n\nw\n") | sudo fdisk $(IMG_DISK)
	sudo losetup -fP $(IMG_DISK)
	sudo mkfs.ext2 $(LOOP_DEVICE)p1
	sudo mount $(LOOP_DEVICE)p1 /mnt
	echo "(hd0) " $(LOOP_DEVICE) > loopdevice.map
	sudo grub-install --target=i386-pc --no-floppy --grub-mkdevicemap=loopdevice.map --fonts="en_US" --themes=no --modules="part_msdos part_gpt" --boot-directory=/mnt $(LOOP_DEVICE) -v
	sudo cp -vf grub/grub.cfg /mnt/grub 
	sudo umount /mnt
	sudo losetup -d $(LOOP_DEVICE)

clean:
	make -C nucleus fclean

fclean:
	make -C nucleus fclean
	rm -vf loopdevice.map
	rm -vf $(IMG_DISK)

re: clean all

copy: $(IMG_DISK)
	sudo dd if=$(IMG_DISK) of=/dev/sdb bs=1024 count=$(IMAGE_SIZE)
	sync

exec:
	qemu-system-x86_64 -m 64 -vga std -hda $(IMG_DISK) -enable-kvm
