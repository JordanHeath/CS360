// This is the echo SERVER server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>

#define  MAX 256

// Define variables:
struct sockaddr_in  server_addr, client_addr, name_addr;
struct hostent *hp;

int  mysock, client_sock;              // socket descriptors
int  serverPort;                     // server port number
int  r, length, n;                   // help variables

// Server initialization code:

int server_init(char *name)
{
   printf("==================== server init ======================\n");   
   // get DOT name and IP address of this host

   printf("1 : get and show server host info\n");
   hp = gethostbyname(name);
   if (hp == 0){
      printf("unknown host\n");
      exit(1);
   }
   printf("    hostname=%s  IP=%s\n",
               hp->h_name,  inet_ntoa(*(long *)hp->h_addr));
  
   //  create a TCP socket by socket() syscall
   printf("2 : create a socket\n");
   mysock = socket(AF_INET, SOCK_STREAM, 0);
   if (mysock < 0){
      printf("socket call failed\n");
      exit(2);
   }

   printf("3 : fill server_addr with host IP and PORT# info\n");
   // initialize the server_addr structure
   server_addr.sin_family = AF_INET;                  // for TCP/IP
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);   // THIS HOST IP address  
   server_addr.sin_port = 0;   // let kernel assign port

   printf("4 : bind socket to host info\n");
   // bind syscall: bind the socket to server_addr info
   r = bind(mysock,(struct sockaddr *)&server_addr, sizeof(server_addr));
   if (r < 0){
       printf("bind failed\n");
       exit(3);
   }

   printf("5 : find out Kernel assigned PORT# and show it\n");
   // find out socket port number (assigned by kernel)
   length = sizeof(name_addr);
   r = getsockname(mysock, (struct sockaddr *)&name_addr, &length);
   if (r < 0){
      printf("get socketname error\n");
      exit(4);
   }

   // show port number
   serverPort = ntohs(name_addr.sin_port);   // convert to host ushort
   printf("    Port=%d\n", serverPort);

   // listen at port with a max. queue of 5 (waiting clients) 
   printf("5 : server is listening ....\n");
   listen(mysock, 5);
   printf("===================== init done =======================\n");
}


main(int argc, char *argv[])
{
   char *hostname;
   char line[MAX];

   if (argc < 2)
      hostname = "localhost";
   else
      hostname = argv[1];
 
   server_init(hostname); 

   // Try to accept a client request
   while(1){
     printf("server: accepting new connection ....\n"); 

     // Try to accept a client connection as descriptor newsock
     length = sizeof(client_addr);
     client_sock = accept(mysock, (struct sockaddr *)&client_addr, &length);
     if (client_sock < 0){
        printf("server: accept error\n");
        exit(1);
     }
     printf("server: accepted a client connection from\n");
     printf("-----------------------------------------------\n");
     printf("        IP=%s  port=%d\n", inet_ntoa(client_addr.sin_addr.s_addr),
                                        ntohs(client_addr.sin_port));
     printf("-----------------------------------------------\n");

     // Processing loop: newsock <----> client
     while(1){
		printf("Waiting for command...\n");
		
		char cmd[256];
		char pathname[256];
	
	
	
       n = read(client_sock, cmd, MAX);
       n = read(client_sock, pathname, MAX);
       

	   
       
       if (n==0){
           printf("server: client died, server loops\n");
           close(client_sock);
           break;
      }
      
      
      
      
      // show the line string
      
       ///printf("%s %s\n\n\n\n",cmd, pathname);
       
       if(!strcmp(cmd, "get"))
       {
		   ///here were going to get first check if the pathname exist
		   struct stat sp;
		   if(stat(pathname, &sp)<0)
		   {
			   printf("FAILED\n\n\n\n");
			   
			   char bad_req[256] = "BAD";
			   n = write(client_sock, bad_req, MAX);
			   //return 0;
			   break;
		   }
		   
		   if(!S_ISREG(sp.st_mode))
		   {
			   char bad_req[256] = "BAD";
			   n = write(client_sock, bad_req, MAX);
			   printf("ISNT REGULAR\n\n\n");
			   break;
		   }
		   else
		   {
			   int size = sp.st_size;
			   char str[256];
			   sprintf(str, "%d", size);
			   n = write(client_sock, str, MAX);
			   
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
						   printf("HERE\n\n\n");
					   }
				   //printf("%s\n\n\n",data);
				   n = write(client_sock, data, MAX);
				   }
				   fclose(infile);   
			   }
		   }
		  
		}
		
		
		
		if(!strcmp(cmd, "put"))
		{
			char temp[256];
			n = read(client_sock, temp, MAX);
			
			printf("size is %s\n", temp);
			
			int numbytes = atoi(temp);
			int count = 0;
			FILE *output_file = fopen(pathname, "w");
			
			while(count<numbytes)
			{
				char buffer[256] = "";
				n = read(client_sock, buffer, MAX);
				count+=n;
				fprintf(output_file, "%s", buffer);
				
				
			}
			fclose(output_file);
			
		}

//END OF THE LINE


printf("Command complete enter new command\n");






    }

 }
}

