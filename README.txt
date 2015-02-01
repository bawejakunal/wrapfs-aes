The source files present in this directory consists of source-code to add to support address_space operations instead of vm_ops operations.

 --------------
| Source files |
 -------------- 
Below files are modified version of original wrapfs
1) wrapfs/wrapfs.h - Added extra fields in wrapfs_sb_info struct to hold mount options. Created a structure which holds the mount options. Added wrapfs_parse_options function which parses the mount options. Added extra file_operations structure.
2) wrapfs/file.c - Added new file operations structure. Modified ->read, ->write, ->unlocked_ioctl to handle encryption-decryption. Modified ->mmap operation to assign address_operations according to the mount options provided.
3) wrapfs/lookup.c - Modified wrapfs_iget() function to make it point to the necessary file_operations and address_operations according to the mount options specified.
4) wrapfs/main.c - Modified wrapfs_read_super() function to store and initialize mount options and key in wrapfs_sb_info structure. Modified wrapfs_mount() function to parse for mount options. The parsing function can recognize the options mmap and debug. The debug mount option is not fully implemented but the parser is having the capability to parse it from the mount command. I did not remove this code as it doesnot affect other parts.
5) wrapfs/mmap.c - Added new address_operations structure and instantiated a dummy address_operations structure to point it according to the mount options. I have copied code from ecryptfs and old unionfs sources. I have used ->readpage, ->write_begin, ->bmap, ->write_end and ->writepage from ecryptfs.
6) wrapfs/super.c - Added wrapfs_parse_options to parse mount options. Necessary structures needed for parsing are also added in this file.
7) wrapfs/Makefile - I have edited the Makefile to take one more flag to include conditional compiling. I have also added commands to compile and clean the user programs accordingly.


The kernel sources can be downloaded from  http://www.kernel.org/pub/linux/kernel/v3.0/linux-3.2.2.tar.bz2
Once downloaded, copy the contents of wrapfs folder to fs/wrapfs/ in kernel sources.


  -------------------------------------------------------------
| Steps to be followed for compiling and executing this project |
  -------------------------------------------------------------
Compiling: 
Go to wrapfs folder
#make

Insert module:
#insmod wrapfs.ko

Mounting:
#mount -t wrapfs -o mmap /mnt/lower /mnt/wrapfs - It takes mmap option to specify whether address operations or not.

Unmounting:
#umount /mnt/wrapfs

Removing module:
#rmmod wrapfs

#make clean  --- will result in removing all the temporary object files.


 -------------------------
| Performance Evaluation |
 -------------------------
 Passed all evaluations from filebench, by FSL Stony Brooks University: http://filebench.sourceforge.net
 
 ------------
| References |
 ------------ 
1) I have copied code from ecryptfs and old unionfs sources. I have used ->readpage, ->write_begin, ->write_end from ecryptfs and ->writepage from unionfs.
