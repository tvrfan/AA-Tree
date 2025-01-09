
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aa_chain.h"

/* orignal in Github JuYanYan/AA-Tree */



/* AA tree with minimum code, modified for use with multiple structs, each in its own tree.

This code requires that structs are declared beginning with NODE at the bginnning and uses casts to
treat NODE as equivalent to start of each data structure. As in -

typedef struct test
{
    NODE node;
    int data1;
    int data2;
    ..etc...
} EXAMPLE;

Delete - swopping nodes is complex as this code does not keep track of 'parent node' which would
be required to relink both of the two nodes. The original code simply swaps the data in the two nodes
and then Skew() and Split() as required. So this code is slightly less efficient, using memcpy to copy the data
of the struct after the NODE.

Note that there is now a COMPARE and a PRINT subroutine for each tree, so TREE and NODE are no longer the same
structure, and must be supplied for each type of STRUCT.

*/

static NODE* Skew(NODE* t)
{
    NODE *l;
    if (t == NULL)     return NULL;
    if (t->left == NULL)   return t;

    if (t->left->level == t->level)
    {
        l = t->left;
        t->left = l->right;
        l->right = t;
        return l;
    }
    return t;
}

static NODE * Split(NODE *t)
{
    NODE *r;
    if (t == NULL)  return NULL;
    if (t->right == NULL || t->right->right == NULL)
        return t;

    if (t->right->right->level == t->level)
    {
        r = t->right;
        t->right = r->left;
        r->left = t;
        r->level++;
        return r;
    }
    return t;
}


static NODE *Find_node (TREE *tree, NODE *node, NODE *data)
{
     int ans ;

    node = tree->root;

    while (node != NULL)
    {
       ans = tree->cmp(data, node);

        if (!ans)
        {
     //     tree->lastnode = node;
          return node;
        }

        if (ans > 0)
            node = node->right;
        else
            node = node->left;
    }
    return NULL;                                           // Not found.
}














static NODE*  Predecessor (NODE*  Node)
{

    Node = Node->left;
    while (Node->right != NULL)
    {

        Node = Node->right;
    }
    return Node;
}

static NODE*  Successor(NODE*  Node)
{

   Node = Node->right;

    while (Node->left != NULL)
    {
           Node = Node->left;
    }
    return Node;                  //need parent of this node also....
}

static NODE * Decrease_level(NODE *t)
{
    int wdo;
    if (t->left != NULL && t->right != NULL)
    {
        wdo = t->left->level > t->right->level ? t->right->level : t->left->level;
        wdo++;

        if (wdo < t->level)
        {
            t->level = wdo;
            if (t->right != NULL && wdo < t->right->level)
            {
                t->right->level = wdo;
            }
        }
    }
    return t;
}

static NODE * InsertData(TREE *x, NODE *t, NODE *data)
{
    int ans;

    if (t == NULL)                   // reached a leaf
    {
  //      t = mallocNode();            // new node to insert
  //      t->data = data;              // void * for anything attached

        t = (NODE*) data;                      //if data is node       THIS WORKS !!
        t->left = NULL;
        t->right = NULL;
        t->level = 1;
    //    x->lastnode = t;            // node inserted
        return t;
    }

    ans = x->cmp(data, t);

 if (ans < 0 )                                      // Find position to insert node.
        t->left = InsertData(x,t->left, data);      // if t->left = null add here
    else if (ans > 0)
        t->right = InsertData(x,t->right, data);   // if t->right = null add here



    t = Skew(t);                                            // Follow the path from the new node to node.
    t = Split(t);                                           // Always do:  Skew and Split.
    return t;
}



static void copy_data(TREE *tree, NODE *a, NODE *b)
{
    char *x, *y;
    int size;

    x = (char*) a + sizeof(NODE);
    y = (char*) b + sizeof(NODE);

    size = tree->esize - sizeof(NODE);

    memcpy (x,y, size);

}




static NODE * DeleteNode(TREE *x, NODE *t, NODE *data)
{
    NODE *m;
    int ans;

    if (t == NULL) return NULL;

    ans = x->cmp(data, t);

    // Find the node to delete.

    if (ans < 0)
    {
        t->left = DeleteNode(x, t->left, data);
    }
    else if (ans > 0)
    {
        t->right = DeleteNode(x, t->right, data);
    }
    else
    {   // ans == 0       .. found node                                         // key == t->key
        if (t->left == NULL && t->right == NULL)            // The node is a leaf. Simply remove it.
        {
            free(t);
            return NULL;
        }


        if (t->left == NULL)                                // Only has right children.
        {
            NODE *l;
            l = Successor(t);                               // get next item by data (not by tree link)
            copy_data(x,t,l);
            t->right = DeleteNode(x, t->right, l);       // Delete successor node.
        }
        else {                                                // has a left child
            NODE *l;
            l = Predecessor(t);                                //get previous item by data
            copy_data(x,t,l);
            t->left = DeleteNode(x, t->left, l);        // delete left node ??
        }

    }
    t = Decrease_level(t);
    t = Skew(t);
    m = t->right;
    t->right = Skew(m);
    if (m != NULL && m->right != NULL)
    {
        t->right->right = Skew(m->right);
    }
    t = Split(t);
    t->right = Split(m);
    return t;
}




static void free_node (NODE *t)
{
    if (t == NULL) return;

    free_node(t->left);
    free_node(t->right);
    free(t);
    t = NULL;               //probably redundant
}


static void PrintNode(TREE *t, NODE *node)
{

    if (!node)   return;

    PrintNode (t, node->right);

    t->prt(t,node,1);

    PrintNode (t, node->left);

}






//external calls





extern void aat_insertData(TREE *t, void *data)
{
     t->root = InsertData (t, t->root, (NODE*) data);                        // To make code simple, using a tiny function to call _InsertData.
}

extern void aat_deleteData(TREE *t, void * data)
{
    DeleteNode (t, t->root, (NODE*) data);
}

extern void* aat_searchData(TREE *tree, void *data)
{

return Find_node (tree, tree->root, (NODE*) data);

}

extern void aat_deleteTree(TREE *t)
{
    if (t == NULL) return;

    free_node(t->root);

}


extern void aat_printTree(TREE *tree)
{

  PrintNode(tree, tree->root);

  printf("\n\n ROOT ");
  tree->prt(tree, tree->root,1);

}
