#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
//#include <time.h>
typedef struct ext2_group_desc GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode INODE;
typedef struct ext2_dir_entry_2 DIR;
GD *gp;
SUPER *sp;
INODE *ip;
DIR *dp;
char buf[1024], dbuf[1024], sbuf[1024];
char newbuf[1024];
int fd;
int iblock, rootblock;
int count = 1;
char *name[256];
int get_block(int fd, int blk, char *buf)
{
	lseek(fd, blk*1024, 0);	
	read(fd, buf, 1024);
}
int search(INODE *qp, char *name)
{
	char *cp;
	//find our directoy
	get_block(fd, qp->i_block[0], dbuf);
	dp = (DIR *)dbuf;//turning into a directory
	cp = dbuf;//assigning string to the file descriptor 
	
	while(cp < &dbuf[1024])
	{
	 strncpy(sbuf, dp->name, dp->name_len);
	 sbuf[dp->name_len] = 0;//NUKK pointer

	 printf("inode = %4d rec_len = %4d name_len = %4d name = %s\n",dp->inode, dp->rec_len, dp->name_len, sbuf);
		if(strcmp(sbuf, name) == 0)
		{//IF WE GET IN HERE, WE HAVE FOUND WHAT WE"RE LOOKING FOR SO WE RETURN
			return dp->inode;
		}
	//cp needs to go up by length of last entry
	 cp += dp->rec_len;//iterating to the next one in line
	 dp = (DIR *)cp;
	}
	printf("FAILED 4.");
	exit(4);

}


 

//default mkdisk name

char *dev = "mydisk";


int dir(char *dev)
{
	int i;
	char *cp;

	fd = open(dev, O_RDONLY);//open

	if(fd < 0)//if not successful open
	{
	printf("open %s FAILED\n", dev);
	exit(1);
	}
	//offset of 1024 bytes is 1 kB.
	get_block(fd, 1, buf);
	sp = (SUPER *)buf;

	
	
	printf("Checkint for ext2 FS on %s.... \n", dev);
	
	if(sp->s_magic != 0xEF53)//check for magic number like he said in textbook
	{
	 printf("%x %s is NOT an exf2 FS\n", sp->s_magic, dev);
	 exit(2);}
	else
	{ printf("It IS an exf2 FS!!\n");}

	//First Group desc is first data block +1 and printing out the info
	get_block(fd, 2, buf);
	gp = (GD *)buf;
	printf("GD Info: %d %d %d %d %d %d\n", gp->bg_block_bitmap,gp->bg_inode_bitmap, gp->bg_inode_table, gp->bg_free_blocks_count, gp->bg_free_inodes_count, gp->bg_used_dirs_count);
	
	
	//The location of the first inode 
	iblock = gp->bg_inode_table;
	printf("inodes beginning block = %d\n", iblock);
	
	//Now were actually reading the block
	get_block(fd, iblock, buf);
	ip = (INODE *)buf + 1;
	
	
	 printf("\n*Root INODE INFO*\n");
	 printf("Mode: %4x ", ip->i_mode);
	 printf("UID: %d gid = %d\n", ip->i_uid, ip->i_gid);
	 printf("Size: %d\n", ip->i_size);
	 printf("Time of: %s", ctime(&ip->i_ctime));
	 printf("Link Count: %d\n", ip->i_links_count);
	 printf("i_block: %d\n\n", ip->i_block[0]);
	
	
	
	for (int j = 0; j < (count); j++)//Goes through and searches 
	{
	
	 int blok;//
	 int offst;//offset bit
	//run search
	 int ino = search(ip, name[j]);
	
	 if (ino<1)
	 {
	   printf("Search FAILED.");
	   exit(3);
	 }
	
	 blok = (ino - 1)/8 + iblock;
	 offst = (ino - 1)%8;
	 get_block(fd, blok, newbuf);

	 ip = ((INODE *)newbuf) + offst;
	//JUSAT LIKE THE FIST PART OF THE ASSIGNMENT
	 printf("INODE INFO...\n");
	 printf("Mode: %4x ", ip->i_mode);
	 printf("UID: %d gid = %d\n", ip->i_uid, ip->i_gid);
	 printf("Size: %d\n", ip->i_size);
	 printf("Time of: %s", ctime(&ip->i_ctime));
	 printf("Link Count: %d\n", ip->i_links_count);
	 printf("i_block: %d\n", ip->i_block[0]);
}
	printf("We Have successfuly found what were looking for!\n");
}

int main(int argc, char *argv[])
{
      char *tok;
      
	  name[0] = strtok(argv[1], "/");
		printf("%s\n\n", name[0]);
	  while (tok != NULL)
	  {
		 tok = strtok(NULL, "/");
		 name[count] = tok;
		 printf("%s", name[count]);
		 count++;
		 printf("%d", count);
	  }

	  count = count - 1;
	  dir(dev);

}
