Wrapfs-AES
==========

**A Vnode Level Stackable Encrypted File System**

The source files present in this directory consist of source-code to add support for `address_space operations` instead of `vm_ops` operations and `128 bit AES-CTR` encryption of data written to files in the WrapFS file system. Please download the appropriate [Wrapfs](http://wrapfs.filesystems.org/) source files for your use from http://wrapfs.filesystems.org/ 

The attached code has been written for Linux kernel stable version 3.2.62.

###Source Files
Below mentioned files are modified version of original wrapfs:  

1. **wrapfs/wrapfs.h** - Added extra fields in `wrapfs_sb_info` struct to hold mount options and 128 bit key for encryption. Created a structure which holds the mount options. Added `wrapfs_parse_options` function which parses the mount options. Added extra file_operations structure. I also defined `calcualted_md5()` and `encrypt_decrypt()` functions over here to include

2. **wrapfs/file.c** - Added new file operations structure. Modified `->read, ->write, ->unlocked_ioctl` to handle encryption-decryption and address space operations by changing calls to `vfs_read()` and `vfs_write()` to `do_sync_read()` and `do_sync_write()` respectively. Modified `->mmap` operation to assign `address_operations` according to the mount options provided.

3. **wrapfs/lookup.c** - Modified `wrapfs_iget()` function to make it point to the necessary `file_operations` and `address_operations` according to the mount options specified.

4. **wrapfs/main.c** - Modified `wrapfs_read_super()` function to store and initialize mount options and key in `wrapfs_sb_info structure`. Modified `wrapfs_mount()` function to parse for mount options. The parsing function can recognize the options `mmap` and `debug`. The `debug` mount option is not fully implemented but the parser is having the capability to parse it from the mount command. I did not remove this code as it does not affect other parts.

5. **wrapfs/mmap.c** - Added new `address_operations structure` and instantiated a dummy `address_operations` structure to point it according to the mount options. I have taken up some code from `ecryptfs` sources. I have used `->readpage, ->write_begin, ->bmap, ->write_end and ->writepage` from `ecryptfs`. Also appropriate function calls have been made within the `->readpage, ->writepage and ->write_end` functions for encrypting and decrypting the data during reading and writing. Further implementation about encryption and decryption is given in `crypto.c` file.

6. **wrapfs/super.c** - Added `wrapfs_parse_options` to parse mount options. Necessary structures needed for parsing are also added in this file.

7. **wrapfs/Makefile** - I have edited the `Makefile` to take one more flag to include conditional compiling. I have also added commands to compile and clean the user programs accordingly.

8. **crypto.c** - This is a newly added file which handles the encryption part. It has two functions implemented `calculate_md5()` to calculate the MD5 hash of the key and `encrypt_decrypt()` to encrypt or decrypt the data passed into it depending on the corresponding option passed. **Pass 1 for encryption and 0 for decryption.**

The kernel sources can be downloaded from  http://www.kernel.org/pub/linux/kernel/v3.0/linux-3.2.2.tar.bz2

Once downloaded, copy the contents of wrapfs folder to `fs/wrapfs/` in kernel sources or put your own patch from `http://wrapfs.filesystems.org/`

 
###Steps to be followed for compiling and executing this project

Now we have three ways to use this implementation of file system viz.  
1. Run a minimalist original Wrapfs implementation without any address space operations.  
2. Run Wrapfs with address space operations.  
3. Run Wrapfs with AES-CTR encryption using address space operations.  

**Compiling** 
1. `cd /path/to/wrapfs`  
2. `make` or `make all` if you want to use Wrapfs with option 1 or 2 i.e without encryption.  
3. `make CRYPTO+=-DWRAPFS_CRYPTO` to use Wrapfs with encryption which is set as the WRAPFS_CRYPTO flag.

**Insert module**  
Execute `insmod wrapfs.ko` to load the compiled module `wrapfs.ko`.

**Mounting**  
1. To use Wrapfs with option 1: `mount -t wrapfs /mnt/lower /mnt/wrapfs`
2. To use Wrapfs with option 2 or 3: `mount -t wrapfs -o mmap /mnt/lower /mnt/wrapfs`. It takes mmap option to specify the use of address space operations.

**Use encryption**  
In case you are using encryption you need to pass a `passphrase` to set the `encryption key`. Do that as follows:

1. `./wrapfs_setkey -k passphrasevalue /mnt/wrapfs` to set the key and subsequently write or read from filesystem
2. `./wrapfs_setkey -k 0 /mnt/wrapfs`, passing a string of one or more zeroes as passphrase will revoke the key.

**Unmounting**  
`umount /mnt/wrapfs` to dismount the filesystem.

**Removing module**  
`rmmod wrapfs` to remove the loadable kernel module from memory.

###Performance Evaluation
Passed all evaluations from filebench, by FSL Stony Brooks University: http://filebench.sourceforge.net
The performance numbers can be checked here: https://docs.google.com/document/d/1Lxo-JuuGSUeWKJtaE_1KToh6VwqBv0HENnt90rmF-vM/edit?usp=sharing
 
###References
 
1. I have referred to code from `ecryptfs` and `old unionfs` source codes for implementation of address space operations. 

2. I have referred `->readpage, ->write_begin, ->write_end` from `ecryptfs` and `->writepage` from unionfs.

####Help
In case you feel that my code is not clear enough or have some doubts regarding implementation etc feel free to contact me on bawejakunal15@gmail.com :)
