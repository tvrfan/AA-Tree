
//modifeid from web

typedef struct  node
{
   // void           *data;
    struct node    *left;
    struct node    *right;
    int             level;
} NODE;


typedef struct test
{
    NODE node;
    int data;
    int data2;
    uint data3;
} TDAT;

typedef struct tree
{
    NODE* root;

    int (*cmp) (void *a, void *b);
    int (*prt) (struct tree *t,NODE *n, int x);
    int esize;
    //more here....
} TREE;




extern void aat_insertData  (TREE *,  void* data);
extern void aat_deleteData  (TREE *,  void* data);
extern void *aat_searchData (TREE *,  void* data);
extern void aat_deleteTree  (TREE *);
extern void aat_printTree   (TREE *);
