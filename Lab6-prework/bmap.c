#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <sys/syscall.h>
#include <unistd.h>


typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp; 


#define MAX 1024
char buf[MAX];
int fd; //file descriptor

char *disk = "mydisk";

int get_block(int fd, int blk, char buf[])
{
	lseek(fd, (long)blk*MAX,0);
	read(fd, buf, MAX);
}

int test_bit(char *buf, int bit)
{
	int i, k;
	i = bit/8;
	k = bit %8;
	return (buf[i] & (1<<k));
}

int bmap()
{
	///if problems arise come here
	int bnode, bmap,i;
	bmap =0;
	
	//Read from the SUPER block...
	get_block(fd, 1, buf);
	sp = (SUPER *)buf; //translate struct
	
	//GET BLOCKS COUNT
	bnode = sp->s_blocks_count;
	printf("NUMBER OF BNODES = %d\n", bnode);
	
	//READ FRO GROUP DESCRIPTOR 0
	get_block(fd, 2, buf);
	gp = (GD *)buf;//redefining
	
	//. Get bmap and print out the given map
	bmap = gp->bg_block_bitmap;
	printf("bmap = %d\n", bmap);
	
	// get the block from bmap and put in buffer
	get_block(fd, bmap, buf);
	
	//6. Using a for loop, print out the bitmap of blocks in 0's and 1's
	for(int i =0;i < bnode; i++)
	{
		(test_bit(buf, i)) ? putchar('1') : putchar('0');
		if(i && (i % 8) ==0)
		{
			printf(" ");
		}	
	}
	printf("\n");
	
}

int main(int argc, char *argv[])
{
	if(argc > 1)
	{
		disk = argv[1];
	}
	
	fd = open(disk, O_RDONLY);
	
	if(fd < 0)
	{
		printf("FAILED OPENING\n");
		exit(0);
	}
	bmap();
	
	
	
}

