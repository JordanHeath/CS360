#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h> // mkdir and such
#include <unistd.h> // to be able to use fork
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <wait.h>



#define MAX_ARGS 64
#define MAX_CHILDREN 32
#define MAX_COMMAND_LENGTH 256


char line[128];
char input[128];
int num_paths;
int filetype;
char *iohandle= NULL;
char *command;
char cwd[1024];
char home[1024];
char **myparse(char *cmd, char delim);
char *get(char *fom);
int file_exist(char* fname);
void process_command(char** args, char* env[], char** paths);
void pipeline_cmd(char *cmd1, char *cmd2);




int main(int argc, char *argv[], char *env[])
{
	//int pid=0;
	strcpy(home ,env[35] + 5);
	//int fd[2];
	//
	///if this goes wrong make it global

	//char **args;
	
	printf("HOME: %s\n", home);
	printf("%s\n", env[21]);
	char **paths =  myparse((env[21] + 5), ':');
	
	
	
	int i = 0;

		
		

	
	//env[20] is PATH
	//env[35] is HOME
				
	
	for(i=0;paths[i];i++)
	{
	
		printf("path %d:%s\n",i, paths[i]);
		
	}
		

	
	
	
	
	getcwd(cwd, sizeof(cwd));
	while(1)
	{
		line[0] = 0;
		fflush(stdin);
		int cmd_count=0;
		int pd[2];
		printf("Jordan-Heath-sh:~%s$ ", cwd);
		fgets(line, 127,  stdin);
		
		
		if(memchr(line, '<', 128))
		{
			printf("THERE IS A < IN THE EQUATION:\n");
			filetype = 1;
			iohandle = memchr(line, '<', 128);
			iohandle[strlen(iohandle)-1]=0;
			
			printf("IOHANDLE: %s",iohandle);
		}
		if(memchr(line, '>', 128))
		{
			
			printf("THERE IS A > IN THE EQUATION:\n");
			filetype = 2;
			iohandle = memchr(line, '>', 128);
			iohandle[strlen(iohandle)-1]=0;
			
			
			printf("IOHANDLE: %s",iohandle);
		}
		if(strstr(line, ">>"))
		{
			
			printf("THERE IS A >> IN THE EQUATION:\n");
			filetype = 3;
			iohandle = strstr(line, ">>");
			iohandle[strlen(iohandle)-1]=0;
		}
		
		
		
		if(filetype == 0)
		{
		line[strlen(line)-1] = 0;//this gets rid of the /r
		}
		else
		{
			printf("sidjfoiansdiofn\n\n");
		line[strlen(line)] = 0;// dont know why but it doesnt acount the \r when i go
		}
		
		//printf("%s\n", line);
		
		char **pipes = myparse(line, '|');
		
		
		while(pipes[cmd_count]!=NULL)
		{
			cmd_count++;
		}
		
		int num_pipes = cmd_count -1;
		
		if(num_pipes>0)
		{
			
			printf(" There are %d pipes \n", num_pipes);
			
			
			
			for(int i=0;pipes[i];i++)
			{
			char **args = myparse(pipes[0], ' ');//seperate out by space
	
		process_command(args, env, paths);//normal execution
		
		sleep(1);
			}
			
			
			
	
			
		}
		else
		{///for normal execution
			//printf("pipe %d:%s\n",i, pipes[i]);
		char **args = myparse(pipes[0], ' ');//seperate out by space
	
		process_command(args, env, paths);//normal execution
		
		sleep(1);
			
		}
		
		//pipes = NULL;
		filetype = 0;
		line[0] = 0;
		
			
		
	}	
	
	
	
	
}


char **myparse(char *cmd, char delim)
{
	int count = 0;
	char **path = (char**) malloc(sizeof(char**));
	char *token = strtok(cmd, &delim);

	while(token)
	{
		printf("TOKEN[%d]: %s\n",count, token);
		if((token != ""))///IF SOMETHING DGOES WRONG, eraise all these tokens
		{
			
		path[count] = (char*)malloc(sizeof(token));
		path[count]=token; // assign 
		count++;		//iterate
		}
		token = strtok(0,&delim);// goes to next
	}
	num_paths = count;

	path[count] = NULL;

	return path;
	
	
}

void process_command(char** args, char* env[], char** paths)
{
	
	int status = 1;
	
		int k =0;//keepts track of how many args we have
		while(args[k])
		{
			if(k==0)
			{
			printf("\t cmd=%s\n", args[k]);
			}
			else
			{
			printf("\t \targ[%d]:%s\n", k, args[k]);
			}
			k++;
		}
		
		
		
		if(strcmp(args[0], "cd")==0)
		{
			if(args[1] == NULL)
			{
				chdir(home);
			}
			else
			{
				printf("%s", args[1]);
				chdir(args[1]);//goes to what ever directory
			}
			getcwd(cwd, sizeof(cwd));//change directory
		}
		else if(strcmp(args[0], "exit") ==0)
		{
			printf("Goodbye\n\n");
			exit(0);
		}
		else
		{//lll other commands
			int exitcode=-1;
			status =1;
			command = args[0];
			int i =0;
			
			
			for(i=0;(i<10)&&(status!=0);i++)
			{
				//pause(2);
				char *path = (char*)malloc(sizeof(paths[i]+1));
				strcpy(path, paths[i]);
				path[strlen(paths[i])]='/';
				path[strlen(paths[i])+1]=0;
				
				args[0] = strcat(path, command);
				
				//printf("Arg[0] = %s\n", args[0]);				
				//printf("Arg[0] = %s\n", args[0]);				
				
				
				pid_t pid;
				switch(pid=fork())
				{
					case -1:
					perror("fork()");
					exit(EXIT_FAILURE);
						break;
					case 0://this is the case where we are allowed to execute the command 
						if(filetype ==1)
						{
							close(0);
							
						filetype=0;
						}
						if(filetype==2)
						{
							//printf("handle:%s\n",iohandle+2);
							close(1);
							open(iohandle+2, O_WRONLY|O_CREAT, 0644);
							
						filetype=0;
						}
						if(filetype==3)
						{
							close(1);
							open(iohandle+2, O_RDWR|O_APPEND, 0644);
							
						filetype=0;
						}
						status= execve(args[0], args, env);
						printf("%d", status);
						exit(status);
					default:
						exitcode = status;
					if(waitpid(pid, &status, 0)< 0)
					{
						perror("waitpid()");
						exit(EXIT_FAILURE);
					}
					
					if(WIFEXITED(status))
					{
						//printf("Childs exit code: %d\n", status);
						//exitcode = status;
						break;
					}
					exit(EXIT_FAILURE);
				}
				
			}
			printf("Childs exit with code: %d\n", exitcode);
			
			
			
		}
			
	
	
	
}


void pipeline_cmd(char *cmd1, char *cmd2)
{
	
	
	
	
	
	
	
}

