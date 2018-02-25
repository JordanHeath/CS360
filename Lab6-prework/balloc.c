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

typedef unsigned int u32;


GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp; 


#define MAX 1024
char buf[MAX];
int fd;
int imap, bmap;  // IMAP and BMAP block number
int ninodes, nblocks, nfreeInodes, nfreeBlocks;


int get_block(int fd, int blk, char buf[ ])
{
  lseek(fd, (long)blk*MAX, 0);
  read(fd, buf, MAX);
}

int put_block(int fd, int blk, char buf[ ])
{
  lseek(fd, (long)blk*MAX, 0);
  write(fd, buf, MAX);
}

int tst_bit(char *buf, int bit)
{
  int i, j;
  i = bit/8; j=bit%8;
  if (buf[i] & (1 << j))
     return 1;
  return 0;
}

int set_bit(char *buf, int bit)
{
  int i, j;
  i = bit/8; j=bit%8;
  buf[i] |= (1 << j);
}

int clr_bit(char *buf, int bit)
{
  int i, j;
  i = bit/8; j=bit%8;
  buf[i] &= ~(1 << j);
}


int decFreeBlocks(int dev)
{
  // I DID IT... maybe...

	//dec free block count in SUPER and GD
	get_block(dev, 1, buf);
	sp = (SUPER*)buf;
	sp->s_free_blocks_count--;
	put_block(dev, 1, buf);

	get_block(dev, 2, buf);
	gp = (GD*)buf;
	gp->bg_free_blocks_count--;
	put_block(dev, 2, buf);
}

u32 balloc(int dev)
{
  // I DID IT... maybe...
	int i;
	printf("allocating block...\n");
 	// get block Bitmap into buf
 	get_block(dev, bmap, buf);
 
 	for (i=0; i < nblocks; i++){
	   if (tst_bit(buf, i)==0){
		 set_bit(buf, i);
		 put_block(dev, bmap, buf);

		 // update free block count in SUPER and GD
		 decFreeBlocks(dev);
		 
		 printf("balloc: block=%d\n", i+1);
		 return (i+1);
	   }
	}
}
char *disk = "mydisk";

int main(int argc, char *argv[])
{
	int i, ino;
  char buf[MAX];

  if (argc > 1)
    disk = argv[1];

  fd = open(disk, O_RDWR);
  if (fd < 0){
    printf("open %s failed\n", disk);
    exit(1);
  }

  // read SUPER block
  get_block(fd, 1, buf);
  sp = (SUPER *)buf;

  ninodes = sp->s_inodes_count;
  nblocks = sp->s_blocks_count;
  nfreeInodes = sp->s_free_inodes_count;
  nfreeBlocks = sp->s_free_blocks_count;
  printf("ninodes=%d nblocks=%d nfreeInodes=%d nfreeBlocks=%d\n", 
	 ninodes, nblocks, nfreeInodes, nfreeBlocks);

  // read Group Descriptor 0
  get_block(fd, 2, buf);
  gp = (GD *)buf;

  imap = gp->bg_inode_bitmap;
  printf("imap = %d\n", imap);
  getchar();

  for (i=0; i < 5; i++){  
    ino = balloc(fd);
    //printf("allocated ino = %d\n", ino);
  }
	
	
}



