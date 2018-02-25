#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>

typedef unsigned int   u32;

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;    // need this for new version of e2fs

GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp; 

/******************* in <ext2fs/ext2_fs.h>*******************************
struct ext2_super_block {
  u32  s_inodes_count;       // total number of inodes
  u32  s_blocks_count;       // total number of blocks
  u32  s_r_blocks_count;     
  u32  s_free_blocks_count;  // current number of free blocks
  u32  s_free_inodes_count;  // current number of free inodes 
  u32  s_first_data_block;   // first data block in this group
  u32  s_log_block_size;     // 0 for 1KB block size
  u32  s_log_frag_size;
  u32  s_blocks_per_group;   // 8192 blocks per group 
  u32  s_frags_per_group;
  u32  s_inodes_per_group;    
  u32  s_mtime;
  u32  s_wtime;
  u16  s_mnt_count;          // number of times mounted 
  u16  s_max_mnt_count;      // mount limit
  u16  s_magic;              // 0xEF53
  // MANY MORE non-essential fields
};
**********************************************************************/

#define BLKSIZE 1024
char buf[BLKSIZE];
char buf2[BLKSIZE];
int ubuf[256];
int ubuf2[256];
int fd;

int get_block(int fd, int blk, char buf1[ ])
{
  lseek(fd, (long)blk*BLKSIZE, 0);
  read(fd, buf1, BLKSIZE);
}

u32 search_inode(INODE* inodePtr, char* name)
{
	int i = 0;
	for(i = 0; i < 12; i++)
	{
		get_block(fd, inodePtr->i_block[i], buf);
		dp = (DIR *)buf;
		strncpy(buf, dp->name, dp->name_len);
		printf("i_block[%d]: %s\n\n", i, inodePtr->i_block[i]);
		//dp = (dir_entry *)((char *)dp + dp->rlen);
	}

}

int showblock(char* path)
{
	u32* blk_ptr;
	int ino, blk, offset;
	int InodesBeginBlock, INODES_PER_BLOCK;
	char* dir_name = NULL;
	char* delim = "/";

	// read SUPER block
	get_block(fd, 1, buf);  
	sp = (SUPER *)buf;

	if (sp->s_magic != 0xEF53)
	{
		printf("NOT an EXT2 FS\n");
    	exit(1);
  	}

	INODES_PER_BLOCK = (1024 << sp->s_log_block_size)/sizeof(INODE);
	printf("BLOCK COUNT: %d\n", sp->s_blocks_count);

	get_block(fd, 2, buf);
	gp = (GD *)buf;

	InodesBeginBlock = gp->bg_inode_table;
	get_block(fd, InodesBeginBlock, buf);
	ip = (INODE *)buf + 1;


	dir_name = strtok(path, delim);

	while(dir_name != NULL)
	{
		int i = 0, j = 0, k =0, file_found = 0;
		for(i = 0; i < 12; i++)
		{
			if(ip->i_block[i] != NULL)
			{
				get_block(fd, ip->i_block[i], buf);
				dp = (DIR *)buf;
				
				while(strcmp(dir_name, dp->name) != 0 && dp->file_type != 8)
				{
					dp = (DIR *)((char *)dp + dp->rec_len);
				}

				if(dp->name != NULL)
				{
					if(dp->file_type == 1)
					{
						file_found = 1;
					}

					ino = dp->inode;
					blk = (ino - 1)/INODES_PER_BLOCK + InodesBeginBlock;
					offset = (ino - 1)%INODES_PER_BLOCK;
					
					get_block(fd, blk, buf);
					ip = (INODE *)buf + offset;	
				}
				
				//destination
				if(file_found == 1)
				{	
					printf("************* DIsk Blocks *************n");
					for(j = 0; j < 14; j++)
					{
						printf("block [%d] = %d\n", j, ip->i_block[j]);
					}

					printf("################# Direct Blocks #################\n");
					for(j = 0; j < 12; j++)
					{
						printf("%d ", ip->i_block[j]);
						if((j + 1)%10 == 0)
							printf("\n");
							
					}

					//INDIRECT BLOCK
					printf("\n\n################# Indirect Blocks #################\n");

					get_block(fd, ip->i_block[12], ubuf);
						 
					int z = 0;
					while(ubuf[z] && ubuf[z] >= ubuf[0])
					{
						
						printf("%d ", ubuf[z]);
						if((z+1)%10 == 0)
							printf("\n");
				
						z++;
					}
					
					memset(ubuf, 0, BLKSIZE);

					//DOUBLY INDIRECT BLOCK
					//INDIRECT BLOCK
					printf("\n\n######### Doubly Indirect Blocks #########\n");

					get_block(fd, ip->i_block[13], ubuf);
						
					j = 0; 
					z = 0;
					while(ubuf[z])
					{
						get_block(fd, ubuf[z], ubuf2);
						while(ubuf2[j])
						{
							printf("%d ", ubuf2[j]);

							if((j + z*12 + 1)%10 == 0)
								printf("\n");
						
							j++;
						}
						z++;
					}
					printf("\n");
					break;					
				}
						
			}	

		}
		
		dir_name = strtok(NULL, delim);
	}

}



main(int argc, char *argv[ ])

{ 
	char* mydisk;
	char* path;

	if (argc > 2)
	{
		mydisk = argv[1];
		fd = open(mydisk, O_RDONLY);
		if (fd >= 0)
		{
			path = argv[2];
			if(path != NULL)
			{
				showblock(path);
			}
		}
		else
		{
			printf("DISK FAILED TO BE OPENED\n");
		}
	}
	else
	{
		printf("INSUFFICIENT ARGUMENTS\n");
	}


}
