// The echo client client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>

#define MAX 256

// Define variables
struct hostent *hp;              
struct sockaddr_in  server_addr; 

int server_sock, r;
int SERVER_IP, SERVER_PORT; 


// clinet initialization code

int client_init(char *argv[])
{
  printf("======= clinet init ==========\n");

  printf("1 : get server info\n");
  hp = gethostbyname(argv[1]);
  if (hp==0){
     printf("unknown host %s\n", argv[1]);
     exit(1);
  }

  SERVER_IP   = *(long *)hp->h_addr;
  SERVER_PORT = atoi(argv[2]);

  printf("2 : create a TCP socket\n");
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock<0){
     printf("socket call failed\n");
     exit(2);
  }

  printf("3 : fill server_addr with server's IP and PORT#\n");
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = SERVER_IP;
  server_addr.sin_port = htons(SERVER_PORT);

  // Connect to server
  printf("4 : connecting to server ....\n");
  r = connect(server_sock,(struct sockaddr *)&server_addr, sizeof(server_addr));
  if (r < 0){
     printf("connect failed\n");
     exit(1);
  }

  printf("5 : connected OK to \007\n"); 
  printf("---------------------------------------------------------\n");
  printf("hostname=%s  IP=%s  PORT=%d\n", 
          hp->h_name, inet_ntoa(SERVER_IP), SERVER_PORT);
  printf("---------------------------------------------------------\n");

  printf("========= init done ==========\n");
}

main(int argc, char *argv[ ])
{
  int n;
  char line[MAX], ans[MAX];

  if (argc < 3){
     printf("Usage : client ServerName SeverPort\n");
     exit(1);
  }

  client_init(argv);
  // sock <---> server
  printf("********  processing loop  *********\n");
  while (1){
    printf("input a line : ");
    bzero(line, MAX);                // zero out line[ ]
    fgets(line, MAX, stdin);         // get a line (end with \n) from stdin

	line[strlen(line)-1] = 0;        // kill \n at end
    if (line[0]==0)                  // exit if NULL line
       exit(0);


	///Split the line up 
	char cmd[256];
	char pathname[256];
	
	
	sscanf(line, "%s %s", cmd, pathname);
	
	
	
	if(!strcmp(cmd, "get"))
	{
		n = write(server_sock, cmd, MAX);
		n = write(server_sock, pathname, MAX);
		//printf("%s\n\n\n\n",cmd);
		///NOW WE WAIT FOR EITHER A SIZE OF A BAD WARNING
		char returnreq[256];
		n = read(server_sock, returnreq, MAX);
		
		if(!strcmp(returnreq, "BAD"))
		{
			printf("something went bad\n");
			//we go to the next command then
		}
		else//the server successfully found the file
		{
			printf("size of %s: %s BYTES\n",pathname, returnreq);
			int numbytes = atoi(returnreq);
			int count = 0;
			FILE *output_file = fopen(pathname, "w");
			
			while(count<numbytes)
			{
				char buffer[256] = "";
				n = read(server_sock, buffer, MAX);
				count+=n;
				fprintf(output_file, "%s", buffer);
				
			}
			fclose(output_file);
		}	
	}
	else if(!strcmp(cmd, "put"))
	{
		
		struct stat sp;
		if(stat(pathname, &sp)<0)
		{
			printf("Something went wrong");
			
		}
		else
		{
			if(!S_ISREG(sp.st_mode))
			{
				printf("ISNT A REGULAR FILE");	
			}
			else
			{//NOW WE CAN SEND THE COMMAND BECAUSE IT EXISTS ON OUR SIDE
				char temp[256];
				int size = sp.st_size;
				sprintf(temp, "%d", size);
				
				
				
				printf("size is %s\n",temp);
				n = write(server_sock, cmd, MAX);
				n = write(server_sock, pathname, MAX);//not needed 
				
				n = write(server_sock, temp, MAX);//Sending the size
				
				FILE *infile = fopen(pathname, "r");
				if(infile)
				{
					int i;
				   int j=0;
				   for(j=0;j<size;j+=256)
				   {
						char data[256] = "";
						char c;
					   for(i=0;i<256 && (c=getc(infile))!=EOF;i++)
					   {
					   data[i]=c;
					   }
					   if(c == "")
					   {
						   printf("HERE\n\n\n\n\n", c);
					   }
				   //printf("%s\n\n\n",data);
				   n = write(server_sock, data, MAX);
				   }
				   fclose(infile);
				}
			}
				
		}
			
	}

   
  }
}
