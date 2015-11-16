eyeNode
A simple file system implemented for CSE231 (Operating Systems) at IIITD, Monsoon 15.


Organisation:
The file system is maintained in blocks, with file support. Each file corresponds to a specific inode in the disk.


Layout:
The disk is laid out as follows:
1 block of data = 4 B

Block 0:
This is the super block. It stored Meta Data that can be used to identify the file system. Eg. The name of the file system "EyeNode FS"

Block 1: 
This contains the inode bitmap.

Block 2:
This Contains the data bitmap.

Blocks 3 to 130:
These contain the INode Data

Blocks 130 - End:
These are the actual data blocks.


Functionality Supported:
int createSFS(char* filename, int nbytes)
Creates a disk with the name filename of bytes nbytes and return the resultant file descriptor

