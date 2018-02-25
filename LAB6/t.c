#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>


///MACRO DEFINITIONS
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
int fd;

typedef enum{false, true}bool;

int get_block(int fd, int blk, char buf[])
{
	lseek(fd, (long)blk*MAX,0);
	read(fd, buf, MAX);
}


char *dirtok[] = {0};



int showblock(char *p)
{
	
	unsigned int* blk_ptr;
	int inode, block, offset;
	int inodebegin, inodes_per;

	char* key = "/";
	int i=0, count;
	
	//1. Read in the SUPER block
	get_block(fd, 1, buf);//offset of 1 byte is super
	sp = (SUPER *)buf;
	
	if(sp->s_magic != 0xEF53)
	{
		printf("This isn't an EXT2 File Sytem.. please try again\n");
		exit(1);
	}
	printf("BLOCK COUNT: %d\n", sp->s_blocks_count);
	
	inodes_per = (1024 << sp->s_log_block_size)/sizeof(INODE);
	get_block(fd, 2, buf);
	
	gp = (GD *)buf;// Group descriptor
	inodebegin = gp->bg_inode_table;
	get_block(fd, inodebegin, buf);
	ip = (INODE *)buf +1;//the begin block is +1


	
	dirtok[i] = strtok(p, key);
	i++;
	while(dirtok[i] = strtok(0, "/"))
	{//this fills an array of char * with the tokens
		printf("path[%d]=%s\n", i, dirtok[i]);
		count = i;
		i++;
	}
	
	dirtok[i] = NULL;
	i=0;
	
	
	while(dirtok[i])
	{
		bool found;
		
		
		
		//each block only has 12 data blocks
		for(int j =0;j<12; j++)
		{
			if(ip->i_block[j])//
			{
				get_block(fd, ip->i_block[j], buf);
				dp = (DIR *)buf;
				
				while(dp->file_type != 8 && strcmp(dirtok[i], dp->name) != 0)
				{
					dp = (DIR*)((char *)dp + dp->rec_len);
				}
				
				if(dp->name != NULL)//if not null, we must have found something
				{
					if(dp->file_type ==1)//this means we found the file
					{
						found = true;
						printf("we found something\n");
					}
					inode = dp->inode;
					block = (inode-1)/inodes_per + inodebegin;
					offset = (inode -1)%inodes_per;
					
					get_block(fd, block, buf);
					ip = (INODE*)buf+offset;
					
				}
			}
		}
		i++;
	}
}












char *mydisk;
char *path;

int main(int argc, char *argv[])
{
	if(argc>2)
	{
		mydisk = argv[1];
		fd = open(mydisk, O_RDONLY);
		if(fd >= 0)//if successful open
		{
			path = argv[2];
			if(path)
			{
				showblock(path);
			}
		}
		else
		{
			printf("wasn't able to print \bs \n");
		}
	}
	else
	{
		printf("Not enough arguements\n");
	}
	
	
	
	
	
}
