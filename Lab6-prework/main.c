#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <fcntl.h>

 

//Check if this is available.

#include <ext2fs/ext2_fs.h>

 

 

//typedef structs

typedef struct ext2_group_desc GD;

typedef struct ext2_super_block SUPER;

typedef struct ext2_inode INODE;

typedef struct ext2_dir_entry_2 DIR;

 

//ptr defines

GD *gp;

SUPER *sp;

INODE *ip;

DIR *dp;

 

 

char buf[1024], dbuf[1024], sbuf[1024];

char newbuf[1024];

char *name[255];

 

int fd;

int iblock, rootblock;

int count = 1;

 

int search(INODE *qp, char *name);

 

//default mkdisk name

char *dev = "mydisk";

 

int get_block(int fd, int blk, char *buf)

{

              printf("get_block: fd = %d blk = %d buf %x\n", fd, blk, buf);

              lseek(fd, blk*1024, 0);

              read(fd, buf, 1024);

}

 

int dir(char *dev)

{

              int i;

              char *cp;

 

              fd = open(dev, O_RDONLY);

              if(fd < 0)

              {

                             printf("open %s FAILED\n", dev);

                             exit(1);

              }

 

              /*

              Now we read SuperBlock at offset 1024

              */

 

              get_block(fd, 1, buf);

              sp = (SUPER *)buf;

 

              printf("Check ext2 FS on %s ", dev);

              if(sp->s_magic != 0xEF53)

              {

                             printf("%x %s is NOT an exf2 FS\n", sp->s_magic, dev);

                             exit(2);

              }

              else

                             printf("It IS an exf2 FS\n");

 

              /*

              Read GD block at The first data block PLUS 1.

              */

              get_block(fd, 2, buf);

              gp = (GD *)buf;

 

              printf("GD Info: %d %d %d %d %d %d\n",

                                           gp->bg_block_bitmap,

                                           gp->bg_inode_bitmap,

                                           gp->bg_inode_table,

                                           gp->bg_free_blocks_count,

                                           gp->bg_free_inodes_count,

                                           gp->bg_used_dirs_count);

 

              iblock = gp->bg_inode_table;

              printf("inodes beginning block = %d\n", iblock);

 

              /*

              Read for root, which is beginning block + 1. Weird.

              */

              get_block(fd, iblock, buf);

              ip = (INODE *)buf + 1;

 

              printf("*********** ROOT INODE INFO *************\n");

              printf("mode = %4x ", ip->i_mode);

              printf("uid = %d gid = %d\n", ip->i_uid, ip->i_gid);

              printf("size = %d\n", ip->i_size);

              printf("time = %s", ctime(&ip->i_ctime));

              printf("link = %d\n", ip->i_links_count);

              printf("i_block[0] = %d\n", ip->i_block[0]);

              rootblock = ip->i_block[0];

              printf("**************** DONE *******************\n");

 

              for (int j = 0; j < (count); j++)

              {

                             int blok;

                             int offst;

 

 

                             int ino = search(ip, name[j]);

                             if (ino<1)

                             {

                                           printf("FAILED.");

                                           exit(3);

                             }

                             blok = (ino - 1)/8 + iblock;

                             offst = (ino - 1)%8;

                             get_block(fd, blok, newbuf);

 

                            

                             ip = ((INODE *)newbuf) + offst;

                             printf("************** INODE INFO *************\n");

                             printf("mode = %4x ", ip->i_mode);

                             printf("uid = %d gid = %d\n", ip->i_uid, ip->i_gid);

                             printf("size = %d\n", ip->i_size);

                             printf("time = %s", ctime(&ip->i_ctime));

                             printf("link = %d\n", ip->i_links_count);

                             printf("i_block[0] = %d\n", ip->i_block[0]);

                             printf("**************** DONE *******************\n");

 

              }

              printf("Success!\n");

}

 

int search(INODE *qp, char *name)

{

              char *cp;

             

              get_block(fd, qp->i_block[0], dbuf);

              dp = (DIR *)dbuf;

              cp = dbuf;

 

              while(cp < &dbuf[1024])

              {

                             strncpy(sbuf, dp->name, dp->name_len);

                             //Shifty way to NULL terminate.

                             sbuf[dp->name_len] = 0;

                             printf("inode = %4d rec_len = %4d name_len = %4d name = %s\n",

                                                          dp->inode,

                                                          dp->rec_len,

                                                          dp->name_len,

                                                          sbuf);

                             //Move the DIR pointer along.

                             if (strcmp(sbuf, name) == 0)

                             {

                                           return dp->inode;

                             }

                             cp += dp->rec_len;

                             dp = (DIR *)cp;

 

              }

              printf("FAILED 4.");

              exit(4);

}

 

int main(int argc, char *argv[])

{

              char *strtoTok;

              char *token;

             

              name[0] = strtok(argv[1], "/");

              while (token != NULL)

              {

                             token = strtok(NULL, "/");

                             name[count] = token;

                             printf("%s", name[count]);

                             count++;

                             printf("%d", count);

              }

              count = count - 1;

              dir(dev);

 

}
