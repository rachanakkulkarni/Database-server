/*tree.h*/

#define _GNU_SOURCE
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>

#include<assert.h>
#include<errno.h>
#include<sys/resource.h>
#include<sys/time.h>
#include<sys/wait.h>

#define TagRoot 1/*0001*/
#define TagNode 2/*0010*/
#define TagLeaf 4/*0100*/

#define NoError         0
#define ExampleFile     "./wl.txt"
#define ExampleFile2    "./wl.50k.txt"

typedef void* Nullptr;
Nullptr null_ptr = 0;

#define find_last(x)    find_last_linear(x)
#define find_leaf(x,y)  find_leaf_linear(x,y)
#define lookup(x,y)     lookup_linear(x,y)
#define find_node(x)    find_node_linear(x)
#define reterr(x) \
        errno = (x); \
        return null_ptr

#define Print(x) \
        zero(buf, 256);\
        strncpy((char *)buf, (char *)(x), 255);\
        size = (int16)strlen((char *)buf);\
        if(size)\
                write(fd,(char *)buf, size)

typedef unsigned int int32;
typedef unsigned short int int16;
typedef unsigned char int8;
typedef unsigned char Tag;

struct s_node {
        Tag tag;
        struct s_node *north;
        struct s_node *west;
        struct s_leaf *east;
        int8 path[256];

};
typedef struct s_node Node;

struct s_leaf{
        Tag tag;
        union u_tree *west;
        struct s_leaf *east;
        int8 key[128];
        int8 *value;//can be large value therefore a pointer
        int16 size; //size of value

};
typedef struct s_leaf Leaf;

union u_tree {
        Node n;
        Leaf l;
};
typedef union u_tree Tree;

int8 *indent(int16);
void print_tree(int, Tree*);
void zero(int8*, int16);
Leaf *find_leaf_linear(int8*,int8*);
int8 *lookup_linear(int8*, int8*);
Node *find_node_linear(int8*);
Node *create_node(Node*, int8*);
Leaf *find_last_linear(Node*);
Leaf *create_leaf(Node*, int8*, int8*, int16);
 int32 *example_searches(int8*);
 Tree  *example_tree(void);
 int32 *example_leaves(void);
 int8  *example_path(int8);
 int8  *example_duplicate(int8*);
int main(int, char**);
