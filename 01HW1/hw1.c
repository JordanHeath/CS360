#include <stdio.h>
#include <stdlib.h>

int *FP;
int A(int x, int y);
int B(int x, int y);
int C(int x, int y);

main(int argc, char *argv[ ], char *env[ ])
{
  int a,b,c;
  printf("enter main\n");
  
  printf("&argc=%x argv=%x env=%x\n", &argc, argv, env);
  printf("&a=%8x &b=%8x &c=%8x\n", &a, &b, &c);

  a=1; b=2; c=3;
  A(a,b);
  printf("exit main\n");
}

int A(int x, int y)
{
  int d,e,f;
  printf("enter A\n");
  // PRINT ADDRESS OF d, e, f
    printf("&d=%8x &e=%8x &f=%8x\n", &d, &e, &f);
  d=4; e=5; f=6;
  B(d,e);
  printf("exit A\n");
}

int B(int x, int y)
{
  int g,h,i;
  printf("enter B\n");
  // PRINT ADDRESS OF g,h,i
    printf("&g=%8x &h=%8x &i=%8x\n", &g, &h, &i);
  g=7; h=8; i=9;
  C(g,h);
  printf("exit B\n");
}

int C(int x, int y)
{
  int u, v, w, i, *p;

  printf("enter C\n");
  // PRINT ADDRESS OF u,v,w;
    printf("&u=%8x &v=%8x &w=%8x\n", &u, &v, &w);
  u=10; v=11; w=12;

  asm("movl %ebp, FP");    // CPU's ebp register is the FP
  
  // 1. print stack frame link list
  p = FP;
  while(p != 0)
  {
    printf("FP -> %8X ->\n", p);
    p = *p;
  }
  printf("FP -> %8X\n", p);

  // 2. print stack contents from -8(FP) to main()
  i = -8;
  FP -= 8;
  while(i < 98)
  {
    printf("%d(address) -> %8X \t(contents) = %d\n", i, FP, *FP);

    FP++;
    i++;
  }
}
