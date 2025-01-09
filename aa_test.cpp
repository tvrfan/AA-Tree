/*
 |Test program.
*/
#include <iostream>
#include "aa_chain.h"
#include <math.h>



int comp (void *x, void *y)
{
    TDAT *a, *b;
    a = (TDAT*) x;
    b = (TDAT*) y;
    return (a->data) - (b->data);

}

int xprt(TREE *t, NODE *node, int indent)
{
    if (!node) {printf(" NULL");
    return 0;}


 TDAT *x;
  x = (TDAT*) node;

  if (indent)
    {
       int spaces;
       int i;
       spaces = t->root->level - node->level;       //calc indent
       for (i = 0; i < spaces; i++)  printf("          ");
    }
  printf( "%2d (%2d)", x->data, x->node.level);
  if (indent) printf("\n");
return 0;
}

// test !!

 TDAT*  mNode(TREE *x)                 //to move outside
{
    TDAT*  p;
    p = (TDAT*) malloc(x->esize);        //includes node...........
    return p;
}







int main()
{
    TREE aat = {NULL, comp, xprt, sizeof(TDAT) };
    int        n , s;      //, *data;
char c;

char cmd[32];
void * ans;

TDAT *z;


while (1)
{
     printf("\n\n Command: ");

     fgets(cmd, 30, stdin);
     s = 0;

     sscanf(cmd,"%c %d %d", &c, &n, &s);

switch (c)
{
   case 'q':

   aat_deleteTree(&aat);

      return 0;

  case 'i':

    z = mNode(&aat);

    z->data = n;

    aat_insertData(&aat, z);         //data);
    aat_printTree(&aat);
    break;


 case 'l':

    if (!s) s = n;

    for (; n<=s; n++)
    {
          z = mNode(&aat);

    z->data = n;

    aat_insertData(&aat, z);

    }
    aat_printTree(&aat);
    break;







  case 'd':


    z = mNode(&aat);
    z->data = n;


    aat_deleteData(&aat, z);
    aat_printTree(&aat);
   break;

  case 'f':
  case 's':

     z =  mNode(&aat);
     z->data = n;




    ans = aat_searchData(&aat, z);

    if (!ans) printf("not found");
    else {
    z = (TDAT*) ans;
    printf( " Found ");
    xprt(&aat, (NODE*) z, 0);



    }

    break;

 case 'p':

    aat_printTree(&aat);
   break;

 default :
printf(" ??\n");
break;



}  //end switch



} //end while
    aat_deleteTree(&aat);

    return 0;
}

// #endif
