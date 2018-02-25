#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
//#include <file.h>
//#include <stdbool.h>


typedef struct node
{
	char name[64];
	char type;//can only be one type, might make it a boolean
	struct  node *parent,*child,*sibling;
	
}NODE;


//global variables. 
NODE *cwd, *root;
char line[128];//user input
char command[16], pathname[64];//
char dname[64], bname[64];
char mypath[64];


char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm",
               "quit", "help", "?", "menu", "reload", "save", 0};


NODE *in(char *name, char type);
int findCmd(char *comm);
int mymkdir(char *pname, char c);
int myls(char *pname);
int mycd(char *pname);
int myrmv(char *pname, char type);
int mysave();
int pwd();
int rpwd(NODE *n);
char *get_path(NODE*t);
int reload();
int recursivewrite(NODE *n, FILE *fp);






int main(int argc, char *argv[])
{
	
	char tempread[64];
	int i =0;
	NODE *Totalroot = in("/", 'D');//initializes the data structure
	root = Totalroot;//initializes pointer
	cwd = root;
	system("clear");
	printf("Welcome to to Unix Terminal Simulation\n\nFor help, type help\n");
	
	
	//printf("%s", root->name);
	while(i<2)	
	{
		//scanf("%63s", tempread);//grab the first line in the command //printf("%s", tempread);
		fgets(line, 127,  stdin);
		line[strlen(line)-1] = 0;//this gets rid of the /r
		
		
		sscanf(line, "%s %s", command, pathname);//splits it up
		//printf("%s\n%s\n", command, pathname);
		
		
		int isFound = findCmd(command);
		
			switch(isFound)
			{
				case 0: if(mymkdir(pathname, 'D'))
				{
					printf("successful\n\n\n");
				}
					break;
				case 1: myrmv(pathname, 'D');
					break;
				case 2: ///printf("fjksadf");
				myls(pathname);
					break;
				case 3:mycd(pathname);
					break;
				case 4: pwd();
					break;
				case 5: mymkdir(pathname, 'F');
					break;
				case 6:  myrmv(pathname, 'F');
					break;
				case 7: // quit
				
				printf("GOODBYE\n");
				mysave();
				exit(0);
					break;
				case 8://help
				printf("Pick from \nmkdir\trmdir\tcd\tpwd\tcreat\trm\tquit\thelp\tmenu\treload\tsave\t? to print out list\n\n");
					break;
				case 9:// ?
				recursivewrite(root, stdout);
					break;
				case 10:// menu 
					break;
				case 11://reload
				reload();
					break;
					
				case 12: mysave();
				break;
				default: 
					break;
			}
		
		
		line[0] =command[0]=pathname[0]=dname[0]= bname[0]=0;//reseing the whole line
		
	}
	
}


NODE *in(char *name, char type)
{//to initialize the root
	NODE *init = (NODE *)malloc(sizeof(NODE));//init
	//init->name = (char*)malloc(sizeof(name));
	strcpy(init->name, name);//copies the name over
	init->type = type;
	init->parent = NULL;
	init->sibling = NULL;
	
	
	
	return init;
	
}


int findCmd(char *comm)
{
   int i = 0;
   while(cmd[i]){
     if (strcmp(command, cmd[i])==0)
         return i;
     i++;
   }
   return -1;
}

int mymkdir(char *pname, char c)
{
	int count =0;
	int i =0;
	char *path[64];
	NODE *here=cwd;
	
	
	if(pname[0]=='/')
	{
	here = root;
	if(!pname[1])
	{
		printf("please name your directory\n");
		return 0;
	}
	
	}
	

	path[0]= strtok(pathname, "/");
	///path[0][strlen(path[0])-1] = 0;
	//printf("path[%d]=%s\n", count, path[count]);
	count++;
	
	while(path[count] = strtok(0, "/"))
	{//this fills an array of char * with the tokens
		//printf("path[%d]=%s\n", count, path[count]);
		count++;
	}
	
	
	
	
		
		
		
		
	
	
	
	while(i < (count-1))
	{//traverse
		
		here = here->child;
		//printf("%s asdnkjflnasduifnaosid", path[i]);
		while(here)
		{
		if(!strcmp(here->name, path[i]))///FOUND ON LEVEL
		{
			i++;//goes to next level
			break;
		}
		else
		{	
			here = here->sibling;
			//printf("went to sibling\n");
		}
		}
		
		
		if(!here)
		{
			printf("there was a problem adding %s\n", path[count-1]);
			return 0;
		}
		
	}
	
	
		
		
		
		
		
		
		
		
		
		if(here->type == 'F')
		{
			printf("you can not enter contents into a F\n");
			return 0;
		}
		
		get_path(here);
		/////THIS IS WHEN WE FINALLY FIND OUR NODE
		if(here)
		{
			
		NODE *parent = here;
		if(here->child == NULL)
		{
			
			here->child = (NODE*)malloc(sizeof(NODE));
			here->child = in(path[count-1], c);
			here->child->parent = here;
			//printf("%s was made in %s\n\n", here->child->name, mypath);
			return 1;
		}
		else
		{
			
			
			here = here->child;//we gotta go to parents now
			
			if(!strcmp(here->name, path[count-1]))
			{
				printf("there was already ONE\n");
				return 0;
				
			}
			
			
			while(here->sibling)
			{
				here = here->sibling;
				if(!strcmp(here->name, path[count-1]))
				{
				printf("there was already one\n");
				return 0;
				
				}
			}
			
			here->sibling = (NODE*)malloc(sizeof(NODE));
			here->sibling = in(path[count-1], c);
			here->sibling->parent = parent;
			//printf("%s was made in %s\n\n", here->sibling->name, mypath);
			return 1;
		}
	}

	
}



int myls(char *pname)
{
	int count =0;
	int i =0;
	char *path[64];
	NODE *here=cwd;
	
	if(pname[0])
	{
		if(pname[0] =='/')
		{
			here = root;//reseting to root
			//count--;
			if(!pname[1])
			{
				count--;
			}
		}

	
	path[0]= strtok(pathname, "/");
	///path[0][strlen(path[0])-1] = 0;
	///printf("path[%d]=%s\n", count, path[count]);
	count++;
	
	while(path[count] = strtok(0, "/"))
	{//this fills an array of char * with the tokens
		///printf("path[%d]=%s\n", count, path[count]);
		count++;
	}
	
	
	
	//STEP 2
	
	
		
	while(i < (count))
	{//traverse
		here = here->child;
		printf("%s \n\n", path[i]);
		
		
		while(here)
		{
		if(!strcmp(here->name, path[i]))
		{
			if(here->type =='F')
			{
				printf("You can not enter a file like a Directory\n");
				return 0;
			}
			i++;//goes to next level
			break;
		}
		else
		{	
			here = here->sibling;
			//printf("went to sibling\n");
		}
		}
		
		
		if(!here)
		{
			printf("could not find %s\n", path[count-1]);
			return 0;
		}
	}
	
		//	printf("could not find\n", path[count-1]);
	if(here->type == 'F')
	{
		printf("you can not ls a file, please enter directory\n");
		return 0;
	}	
	
	
	
	}
	
	
	
	
	//printf("\n\n"
		printf("*****In Directory %s*****\n", here->name);
		here = here->child;
		
		//
		
		while(here)
		{
			printf("%c\t%s\n",here->type, here->name);
			here = here->sibling;
		}
	
	
	
	
}

int mycd(char *pname)
{
	
	int count =0;
	int i =0;
	char *path[64];
	NODE *here=cwd;
	
	///STEP 2
	
	if(pname[0]==0)
	{
		cwd = root;
		printf("returning to /\n");
		return 1;
	}
	
	
	path[0]= strtok(pathname, "/");
	///path[0][strlen(path[0])-1] = 0;
	///printf("path[%d]=%s\n", count, path[count]);
	count++;
	
	while(path[count] = strtok(0, "/"))
	{//this fills an array of char * with the tokens
		///printf("path[%d]=%s\n", count, path[count]);
		count++;
	}
	
	
	
	//STEP 2
	
	
	if(pname[0] != '/')
	{
		
	while(i < (count))
	{//traverse
		here = here->child;
		printf("%s \n\n", path[i]);
		
		
		while(here)
		{
		if(!strcmp(here->name, path[i]))
		{
			if(here->type =='F')
			{
				printf("You can not enter a file like a Directory\n");
				return 0;
			}
			i++;//goes to next level
			break;
		}
		else
		{	
			here = here->sibling;
			//printf("went to sibling\n");
		}
		}
		
		
		if(!here)
		{
			printf("could not find %s\n", path[count-1]);
			return 0;
		}
	}
	
		//	printf("could not find\n", path[count-1]);
	if(here->type == 'F')
	{
		printf("you can not cd a file, please enter directory\n");
		return 0;
	}
	cwd = here;
		
	}
	else
	{
		here = root;
		///THERES A PROBLEM HERE
		if(!path[0])
		{
			count--;
		}
		
		while(i < (count))
		{//traverse
		here = here->child;
		//printf("%s asdnkjflnasduifnaosid", path[i]);
		
		
		while(here)
		{
		if(!strcmp(here->name, path[i]))
		{
			if(here->type =='F')
			{
				printf("You can not enter a file like a Directory\n");
				return 0;
			}
			i++;//goes to next level
			break;
		}
		else
		{	
			here = here->sibling;
			///printf("went to sibling\n");
		}
		}
		
		
		if(!here)
		{
			printf("could not find %s\n", path[count-1]);
			return 0;
		}
	}
	
		//	printf("could not find\n", path[count-1]);
	if(here->type == 'F')
	{
		printf("you can not cd a file, please enter directory\n");
		return 0;
	}
	cwd = here;
	
		
		
		
	}	
}



int myrmv(char *pname, char type)
{
	
	
	
	int count =0;
	int i =0;
	char *path[64];
	NODE *here=cwd;
	NODE *parent;
	NODE *sibling;
	
	///STEP 2
	
	
	
	path[0]= strtok(pathname, "/");
	///path[0][strlen(path[0])-1] = 0;
	printf("path[%d]=%s\n", count, path[count]);
	count++;
	
	while(path[count] = strtok(0, "/"))
	{//this fills an array of char * with the tokens
		printf("path[%d]=%s\n", count, path[count]);
		count++;
	}
	
	
	
	//STEP 2
	
	
	if(pname[0] != '/')
	{
		
	while(i < (count))
	{//traverse
		parent = here;
		here = here->child;
		printf("%s \n\n", path[i]);
		
		sibling = NULL;
		while(here)
		{
		if(!strcmp(here->name, path[i]))
		{
			i++;//goes to next level
			break;
		}
		else
		{	
			sibling = here;
			here = here->sibling;
			//printf("went to sibling\n");
		}
		}
		
		
		if(!here)
		{
			printf("could not find %s\n", path[count-1]);
			return 0;
		}
	}
	
		//	printf("could not find\n", path[count-1]);
	
	if(!strcmp(here->name, path[count-1]))
	{//check if its equal in name if it is we proced to delete it
		if(here->type != type)
		{
			if(type == 'D')
			{
			printf("you are trying to delete a Directory with the rm function\n");
			}
			else
			{
			printf("you are trying to delete a File with the rmdir function\n");
			}
			return 0;
		}
		
		//printf("Jniasdnofiasndifna");
		if(here->child == NULL)
		{
			if(sibling == NULL)
			{//this means it's direct from the parent
				if(here->sibling)
				{
					parent->child = here->sibling;
					here = NULL;
					printf("successfully deleted\n");
					return 1;
				}
				else
				{
					parent->child = NULL;
					
				}	
			}
			else
			{
				sibling->sibling = here->sibling;//reassign pointers
				here = NULL;
				printf("successfully deleted\n");
				return 1;
				
			}
			
			
			
			
			
		}
		else
		{
			printf("YOU CAN NOT REMOVE A DIRECTORY WITH STUFF IN IT\n");
			return 0;
		}
		
		
		
	}
		
	}
	else
	{
		here = root;
		parent = here;
		///THERES A PROBLEM HERE
		if(!path[0])
		{
			count--;
		}
		
		while(i < (count))
		{//traverse
		parent = here;
		here = here->child;
		//printf("%s asdnkjflnasduifnaosid", path[i]);
		
		sibling = NULL;
		while(here)
		{
		if(!strcmp(here->name, path[i]))
		{
			i++;//goes to next level
			break;
		}
		else
		{	
			sibling = here;
			here = here->sibling;
			//printf("went to sibling\n");
		}
		}
		
		
		if(!here)
		{
			printf("could not find %s\n", path[count-1]);
			return 0;
		}
		
		
		
	if(!strcmp(here->name, path[count-1]))
	{//check if its equal in name if it is we proced to delete it
		
		if(here->type != type)
		{
			if(type == 'D')
			{
			printf("you are trying to delete a Directory with the rm function\n");
			}
			else
			{
			printf("you are trying to delete a File with the rmdir function\n");
			}
			return 0;
		}
		
		//printf("Jniasdnofiasndifna");
		if(here->child == NULL)
		{
			if(sibling == NULL)
			{//this means it's direct from the parent
				if(here->sibling)
				{
					parent->child = here->sibling;
					here = NULL;
					printf("successfully deleted\n");
					return 1;
				}
				else
				{
					parent->child = NULL;
					
				}		
			}
			else
			{
				sibling->sibling = here->sibling;//reassign pointers
				here = NULL;
				printf("successfully deleted\n");
				return 1;
				
			}
			
			
		}
		else
		{
			printf("YOU CAN NOT REMOVE A DIRECTORY WITH STUFF IN IT\n");
			return 0;
		}
		
		
		
	}
		
		
	}
	//	printf("could not find\n", path[count-1]);
	cwd = here;
	}	
}

int pwd()
{
	char *wd = get_path(cwd);
	if(wd)
	{
	printf("%s", get_path(cwd));
	}
	else
	{
		printf("/"); 
	}
	printf("\n");
	mypath[0] = 0;//reset it 
	
}

char *get_path(NODE*t)
{
	if(t==root)
	{
		mypath[0] =0;
		return 0;
	}
	get_path(t->parent);//goes into parent
	strcat(mypath, "/");//adds a /
	strcat(mypath, t->name);//adds name
}

int recursivewrite(NODE *n, FILE *fp)
{
	if(!n)//doesnt open
	{
		return 0;
	}
	get_path(n);//gets the path
	strcat(mypath, "\n");//adds a new line at the emd
	//printf("%c %s", n->type, mypath);//prints to the screen
	if(n !=root)
	{
		fprintf(fp, "%c %s", n->type, mypath);
	}
	recursivewrite(n->child, fp);//recursively go to the left
	recursivewrite(n->sibling, fp);//recursivly go to the right
	
	}

int reload()
{//ENTER FILENAME SETTING
	
	FILE *fp = fopen("myfile.txt", "r+");//file for reading
	
	char temptype;
	if(!fp)
	{
		printf("there was no file of sorts\n");
		return 0;
	}
	
	root = in("/", 'D');//init new root
	
	while(!feof(fp))//go until end of file
	{
		line[0] = 0;//init to clear line at begining
		fgets(line, 128, fp);//get the lines from the file
		if(line[0]== 0 || line[0]=='\n')
		{continue;//deal with new line and line segments 
		}
		sscanf(line, "%c %s", &temptype, pathname);//scan  type, then name
		//printf("%c pathname = %s\n", temptype, pathname);//prints it to screen for conformation
		
		if(temptype == 'D')
		{
			mymkdir(pathname, 'D');//encapsulated 2 different functionswithin 1, does checks as well very easily
		}
		else
		{
			mymkdir(pathname, 'F');
		}
		
	}
	
	cwd = root;
	fclose(fp);

}
int mysave()
{
	FILE *fp = fopen("myfile.txt", "w+");//open safely
	recursivewrite(root, fp);//this writes to the file 
	fclose(fp);//safely close
}
