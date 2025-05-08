/*tree.h*/

#define _GNU_SOURCE
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#include<assert.h>
#include<errno.h>

#define TagRoot 1/*0001*/
#define TagNode 2/*0010*/
#define TagLeaf 4/*0100*/

#define NoError         0

typedef void* Nullptr;
Nullptr null_ptr = 0;

#define find_last(x)    find_last_linear(x)
#define reterr(x) \
        errno = (x); \
        return null_ptr

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
