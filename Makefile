WRAPFS_VERSION="0.2"

EXTRA_CFLAGS += -DWRAPFS_VERSION=\"$(WRAPFS_VERSION)\" $(CRYPTO)

obj-$(CONFIG_WRAP_FS) += wrapfs.o

wrapfs-y := dentry.o file.o inode.o main.o super.o lookup.o mmap.o crypto.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -Wall -Werror wrapfs_setkey.c -o wrapfs_setkey.o
default:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -Wall -Werror wrapfs_setkey.c -o wrapfs_setkey.o
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
