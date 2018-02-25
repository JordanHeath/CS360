#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
typedef unsigned int u32;

typedef struct node
{
	struct node *next;
	char* name;
	int priority;
}NODE;

//int BASE = 0;//WE CHANGE TO 10 for u, 16, for h, 8 for o
char *table = "0123456789ABCDEF";

NODE *readyQueue=NULL;

int enqueue(NODE **queue, NODE *p); // double piointet means it's a pointer to node
NODE *dequeue(NODE **queue);


//typedef unisgned int u32;
int prints(char *s);
int printd(int x);
int printo(u32 x);
int printx(u32 x);
int rpu(u32 x, int base);
//allocating the node
NODE *allocate(int num);
//dealing with the priority queue structure
void buildqueue(NODE **a)
{
	NODE *n = a;
	int i =0;
	
	while(i<10)
	{
		n->name = NULL;
		n->next = NULL;
		n->priority = i;
		n++;//iterates to next one in series
		i++;
	}
	
	
}

void printqueue(NODE **b, int pri);
int myprintf(char * fmt, ...);//this is a function for my print to screen

int main(int argc, char *argv[], char *env[])
{
	
	NODE *p;
	int i =0;
	///system("clear");
	buildqueue(&readyQueue);
	
	
	
	
	
	myprintf("argc = %d\n", argc);
		
	for(i =0; i < argc;i++)
	{
		myprintf("argv[%d] = %s\n \r", i, argv[i]);
		//printf("argv[%d] = %s\n \r", i, argv[i]);
	}

	
	for(i =0; i < 57;i++) // this prints out the env[]
	{
		myprintf("env[%d] = %s\n \r", i, env[i]);
	}
	
	//myprintf("%o\n", 01233523);
	myprintf("\n\n*****WELCOME TO ASSIGNMENT 2*****\n\nTo Enqueue:'e'\nFor Dequeue: 'd'\nTo Print List:'p'\nTo Quit: 'q'\n\n");
		
	
	
	i=0;
	while(1)
	{
		char c = getchar();
		if(c == 'e')
		{
		NODE *n =readyQueue;
		p = allocate(i);
		//printf("%s P-%d\n",p->name, p->priority);
		enqueue(&readyQueue, p);
		myprintf("[%s %d]\n",p->name,p->priority);
		printqueue(&readyQueue,p->priority);
		
		//getchar();
		//myprintf("\n");
		i++;
		}
		else if(c == 'd')
		{
		dequeue(&readyQueue);
		printqueue(&readyQueue, 0);
		}
		else if(c=='q')
		{
			break;
		}
		else if(c=='p')
		{
			printqueue(&readyQueue,0);
			
		}
	
	}
		
		
		
		
		system("clear");
		
		myprintf("Thank you for Using my Application\n\n\n\n\nJordan Heath\nID: 11376098\nCPTS-360\n\n\n", 02341);
		///JUST NEED TO PRINT THE QHOLE QUEUE AT THE END
		
		
		/*
		int j =0;
		while(j < 10)
		{
			printf("P[%d]\n", j);// for the specific on
			NODE *n = &readyQueue[j];
			while(n)
			{
				printf("%s -> ",n->name);
				n = n->next;
			}
			putchar('\n');
			j++;
		}
		
	*/
	 
	 
	
	


} 




int myprintf(char * fmt, ...)
{
	
	char *cp = fmt;
	int *ip = &fmt +1; //going to the ... in the parameter.
	int length = strlen(cp);
	int i=0;
	char *grab;
	
	//va_list a_list;
	//va_start(a_list, fmt);
	//vprintf(fmt,a_list);

	//printf("%s", *ip);





	while(i<length)
	{
		if(cp[i]!='%')
		{
			putchar(cp[i]);//print character
			if(cp[i] == '\n')
			{
				putchar('\r');
			}
		}
		else
		{
			i++;//iterate to next element
			switch(cp[i])
			{
				case 'd': printd(*ip);
						break;
				case 's':  //grab = va_arg(a_list,  char *);
				prints(*ip);
						break;
				case 'c': putchar(*ip);
				break;		
				case 'u': printu(*ip, 10);
				break;
				case 'o': printo(*ip);
				break;
				case 'x': 
				prints("0x");
				printx(*ip);
				break;
						default: break;
			}
			ip++;//iterates the pointer to the next
			
		}
		
		
		
		i++;
	}
	

			
		
		
		
		
		
		
		
	
	
	
	
}

NODE *allocate(int num)
{
	NODE *n = (NODE *)malloc(sizeof(NODE));
	n->name = (char*)malloc(sizeof(char)*64);//allocating 64 
	n->next = NULL;
	n->priority = rand() % 10;
	
	char *name = (char *)malloc(sizeof(char)*32);
	name[0] = 'n';	
	name[1] = 'o';	
	name[2] = 'd';
	name[3] = 'e';
	
	
	if(num > 9)
	{
	int i=0;
	char *stack = (char *)malloc(sizeof(char)*32);
	while(num>9)
	{
		stack[i] = (num % 10)+48;
		//putchar(stack[i]);
		num = num/10;
		i++;
	}
	//putchar(num+48);
		
	int a=1;
	
	name[4] = num+48;

	while(i >= 0)
	{
		name[4+a] = stack[i-1];
		a++;
		i--;
	}
	
}
else
{
	
	name[4] = num + 48;
	name[5] = '\0';
	
}
	strcpy(n->name, name);
	

	

	
	return n;
	
}


int prints(char *s) // for string
{
	int i =0;
		
	for(i=0;i<strlen(s);i++)
	{
		putchar(s[i]);
	}
	
	
	
}

int printd(int x) // for integer
{

	if(x < 0)
	{
		putchar('-');//puts the negative number
		x = x * (-1);// flips the number so we can do it right
	}
	
	if(x=='0')
	{
		putchar('0');
	}

	
	if(x/10)
	{//larget than ten, 
		printd(x/10);
	}
		
	putchar((x%10)+48);
}

int printo(u32 x) // for oct
{
	if(x==0)
	{
		putchar('0');
	}
	else
	{
		putchar('0');//puts the 0 before it
		rpu(x, 8);
	}
	putchar(' ');
}

int printx(u32 x) // for hex
{
	
	if(x==0)
	{
		putchar('0');
	}
	else
	{
		rpu(x, 16);
	}
	putchar(' ');
	
	
}

int printu(u32 x, int base)
{
	if(x==0)
	{
		putchar('0');
		}
	else
	{
		rpu(x, 10);
	}
	putchar(' ');
	
}

int rpu(u32 x, int base)//recursive print unsigned
{
	char c;//define local varalbe
	if(x)
	{
		c = table[x%base];
		rpu(x/base, base);
		putchar(c);
	}
	
}

int enqueue(NODE **queue, NODE *p)
{
	

	NODE *q = queue;//assigning another
	int i=0;
	
	while(q->priority != p->priority)
	{//this goes to find the q Head NODE
		
		q++;
		i++;
		if(i>9)
		{
			myprintf("error occured, terminating\n\n\n");
			exit(1);
		}
		
	}
	
	if(q->name == NULL)
	{//then it is empty for this priority
		// NOW WE WANT TO FILL IT SINCE WE KNOW IT'S NUMM
		q->name = (char*)malloc(sizeof(p->name));
		strcpy(q->name, p->name);
		q->priority=p->priority;
		
	}
	else
	{//this means that we have to go to the next
		
		while(q->next != NULL)
		{
			q = q->next;
		}
		q->next = (NODE*)malloc(sizeof(p));
		q->next->name = (char*)malloc(sizeof(p->name));//dont know if this will work
		strcpy(q->next->name, p->name);
		q->next->priority = p->priority;
		//q->next->next=NULL;//safety for adding mpore
		//printf("%s\n\n\n", q->next->name);
	}
	
}

NODE *dequeue(NODE **queue)
{
	NODE *n = (queue)+27;//iterates to the 9th one, the last one
	//myprintf("%d", n->priority); 
	//were at the highest priority
	
	
	int i=10;
	while(i>0)
	{
		
		if(n->name != NULL)
		{// we wan to delete this one
			myprintf("[%s %d] Dequeued\n",n->name, n->priority);
			NODE *returnme = n;
			if(n->next == NULL)
			{
				n->name = NULL;
			}
			else
			{
				strcpy(n->name, n->next->name);
				n->next = n->next->next;
			}
			break;
		}
		n--;//going down to find it
		i--;
		
	}
	return n;
}


void printqueue(NODE **b, int pri)
{
	NODE *c = b+27;
	
	int i;
	//pause();

	
	myprintf("readyQueue=");
	for(i=10;i>0;i--)
	{//starting with the highest priority
	
		if(c->name != NULL)//means there's contents for this priority
		{
			NODE *n = c;//temp node to go thru this part
			while(n)
			{
				myprintf("[%s %d]->", n->name, n->priority);
				n=n->next;//goes to next
			}
			
		}
		
		c--;
	}
	
	//printf("\n\n\n\%d\n\n\n", i);
	
	
	
	/*
	while(c->priority != pri)
	{
			c++;//goes to next priority
			///need safety check
	}
	myprintf("For Priority %d:\t", c->priority);
	//now we got the priority
	while(c)
	{
		myprintf("%s -> ", c->name);
		c = c->next;//iterating
	}
	*/
	putchar('\n');
	
	
}






