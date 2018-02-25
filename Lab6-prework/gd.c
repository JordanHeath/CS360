#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <sys/syscall.h>
#include <unistd.h>


///MACRO DEFINITIONS
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

//CREATING OUR GLOBAL
GD    *gp;

//DEFINIGN MAX
#define MAX 1024
char buf[MAX];//buffer which will contain out structure
int fd;//file descriptor

int get_block(int fd, int blk, char buf[ ])
{
	lseek(fd, (long)blk*MAX, 0);
	read(fd, buf, MAX);
}

int gd()
{

	get_block(fd, 2, buf);
	gp = (GD *)buf;//the buffer was from the block

	printf("bg block number = %d\n", gp->bg_block_bitmap);
	printf("bg inode bitmap = %d\n", gp->bg_inode_bitmap);
	printf("bg inode table = %d\n", gp->bg_inode_table);
	printf("bg free blocks count = %d\n", gp->bg_free_blocks_count);
	printf("bg free inodes count = %d\n", gp->bg_free_inodes_count);
	printf("bg used dirs count = %d\n", gp->bg_used_dirs_count);

}

char *disk = "mydisk";

int main(int argc, char *argv[ ])
{
	if (argc > 1)
		disk = argv[1];
	fd = open(disk, O_RDONLY);
	if (fd < 0)//if not open corrently...
	{
		printf("open failed\n");
		exit(1);
	}

	printf("Group Descriptor 0 information\n");
	gd();

}
