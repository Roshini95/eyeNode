# eyeNode
A simple file system implemented for CSE231 (Operating Systems) assignment.

###Some specifications

* 128MB virtual drive
* One inode entry : 16 bytes (8 bytes for name, 2 bytes for starting block, 2 bytes for number of blocks, 4 bytes for file size)
* First block : Super bit
* Second block : Inode bitmap
* Third block : Data bitmap
* Next 128 blocks : Inode data
