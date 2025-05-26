/*tree.c*/

#include "tree.h"

Tree root = {.n={
        .tag = (TagRoot | TagNode),
        .north = (Node *)&root,
        .west = 0,
        .east = 0,
        .path = "/"
}};

void print_tree(int fd, Tree *_root){
        int8 indentation;
        int8 buf[256];
        int16 size;
        Node *n;
        Leaf *l;

        indentation = 0;

        for(n=(Node *)_root; n; n=n->west){
                Print(indent(indentation++));
                Print(n->path);
                Print("\n");
                if(n->east)
                        for(l=n->east;
                        l;
                        l=l->east){
                                Print(indent(indentation));
                                Print(n->path);
                                Print("/");
                                Print(l->key);
                                Print(" -> ");
                                write(fd, (char *)l->value, (int)l->size);
                                Print("'\n'");
                        }

        }

        return;
}

int8 *indent(int16 n) {
        int16 i;
        static int8 buf[256]; //can be accesed from outside, but zeroed out in this function
        int8 *p;

        if(n<1)
                return (int8 *)"";

        assert(n<120);
        zero(buf, 256);

        for(i=0, p=buf; i<n; i++, p+=2)
                strncpy((char *)p, "  ", 2);

        return buf;

}

void zero(int8 *str, int16 size){
        int8 *p;
        int16 n;

        for(n = 0, p=str;n<size; p++,n++)
                *p = 0;
}
/*utility function*/
Node *create_node(Node *parent, int8 *path) {
        Node *n;
        int16 size;

        errno = NoError;

        assert(parent);
        size = sizeof(struct s_node);
        n = (Node *)malloc((int)size);
        zero((int8 *)n, size);

        parent->west = n;
        n->tag = TagNode;
        n->north = parent;
        strncpy((char*)n->path, (char*)path, 255);

        return n;

}

Node *find_node_linear(int8 *path){
        Node *p,*ret;

        for(ret=(Node *)0, p=(Node *)&root; p; p=p->west) {
                if(!strcmp((char *)p->path,(char *)path)) {
                        ret = p;
                        break;
                }
        }
                return ret;
}

Leaf *find_leaf_linear(int8 *path, int8 *key){
        Node *n;
        Leaf *l, *ret;

        n = find_node_linear(path);
        if(!n)
                return (Leaf *)0;

        for(ret=(Leaf *)0, l=n->east; l; l=l->east)
                if(!strcmp((char *)l->key, (char *)key)){
                        ret = l;
                        break;
                }
        return ret;
}

int8 *lookup_linear(int8 *path, int8 *key){
        Leaf *p;

        p = find_leaf_linear(path, key);

        return (p) ?
                p->value:
                (int8 *)0;
}

Leaf *find_last_linear(Node *parent) {
        Leaf *l;

        errno = NoError;

        assert(parent);
        if(!parent->east) {
                return(NoError);
        }

        for(l = parent->east; l->east; l=l->east);
        assert(l);

        return l;

}

Leaf *create_leaf(Node *parent, int8 *key, int8 *value, int16 count){
        Leaf *l, *new;
        int16 size;

        assert(parent);
        l = find_last(parent);

        size = sizeof(struct s_leaf);
        new = (Leaf *)malloc(size);

        assert(new);

        if(!l)
                //directly connected
                parent->east = new;
        else
                //l is leaf
                l->east = new;
        zero((int8 *) new, size);
        new->tag = TagLeaf;
        new->west = (!l) ?
                (Tree *)parent :
                (Tree *)l;
        strncpy((char *)new->key, (char *)key, 127);
        new->value = (int8*) malloc(count);
        zero(new->value, count);
        assert(new->value);
        strncpy((char *)new->value, (char *)value, count);
        new->size = count;

        return new;

}

Tree *example_tree() {
        int8 c;
        Node *n,*p;
        int8 path[256];
        int32 x;

        zero(path, 256);
        x = 0;

        for(n =(Node *)&root, c='a';c<='z';c++){
                x = (int32)strlen((char *)path);
                *(path + x++) = '/';
                *(path + x) = c;

                p = n;
                n =  create_node(p, path);
        }

        return (Tree *)&root;

        }

int8 *example_path(int8 path) {
        int32 x;
        static int8 buf[256];
        int8 c;
        zero(buf, 256);

        for(c = 'a'; c<=path; c++) {
                x = (int32)strlen((char *)buf);
                *(buf + x++) = '/';
                *(buf + x) = c;
        }
        return buf;
}

int8 *example_duplicate(int8 *str){
        int16 n, x;
        static int8 buf[256];

        zero(buf, 256);
        strncpy((char *)buf, (char *)str,255);
        n = (int16)strlen((char *)buf);
        x = (n*2);
        if(x>254)
                return buf;
        else
                strncpy((char *)buf + n, strdup((char *)buf),x);
                return buf;

}
/*
int32 *example_leaves (){
        int fd;
        int32 x,y;
        int8 c;
        int8 buf[256];
        int8 *path, *val;
        Leaf *l;
        Node *n;

        fd = open(ExampleFile, O_RDONLY);
        assert(fd);

        zero(buf, 255);
        y = 0;
        while((x = read(fd, buf, 255)) > 0) {
                *(buf+x-1) = 0;
                path = example_path(*buf);
                n = find_node(path);
                if(!n){
                        zero(buf, 256);
                        continue;
                }

                val = example_duplicate(buf);
                l = create_leaf(n,buf,val,
                (int16)strlen((char *)val));
        y++;
        }
        close(fd);

        return (int32 *)y;
        zero(buf, 256);
}
*/

int32 *example_leaves() {
        int fd;
        int32 x, y;
        int8 buf[256];
        int8 *path, *val;
        Leaf *l;
        Node *n;

        fd = open(ExampleFile, O_RDONLY);
        assert(fd);

        zero(buf, 256);
        y = 0;
        while ((x = read(fd, buf, 255)) > 0) {
                buf[x] = 0; // Null-terminate
                // Assume file contains one lowercase letter per read
                if (x >= 1 && buf[0] >= 'a' && buf[0] <= 'z') {
                        path = example_path(buf[0]); // Single character
                        n = find_node_linear(path);
                        if (!n) {
                                zero(buf, 256);
                                continue;
                        }

                        // Use first word as key
                        int8 key[128];
                        zero(key, 128);
                        strncpy((char *)key, (char *)buf, strcspn((char *)buf, "\n "));
                        val = example_duplicate(key);
                        l = create_leaf(n, key, val, (int16)strlen((char *)val));
                        y++;
                }
                zero(buf, 256); // Clear for next read
        }

        close(fd);
        return &y; // Fix return type to match int32*
}

int32 *example_searches(int8 *file) {
        FILE *fd;
        int8 buf[64];
        int8 *path, *value;
        int16 size;
        int32 n;

        fd = fopen((int8 *)file, "r");
        assert(fd);

        n = 0;

        zero(buf, 64);
        while(fgets((char *)buf, 63, fd)){
                size = (int16)strlen((char *)buf);
                assert(size);
                size--;
                buf[size] = 0;

                path  = example_path(*buf);
                value = lookup(path, buf);
                if(value) {
                        printf("%s -> '%s' \n", buf, value);
                        fflush(stdout);
                        n++;
                }
                zero(buf, 64);
        }
        fclose(fd);

        return n;
}

/*int main(int argc, char *argv[]) {
        Tree *example;
        int32 x,y;
        int8 *p;
        struct rusage usage;
        float duration;
        int8 *file;

        if(argc < 2){
                fprintf(stderr, "Usage: %s INFILE\n", *argv);
                return -1;
        }
        else
                file = (int8 *)argv[1];

        example = example_tree();
        printf("Populating the tree... ");
        fflush(stdout);
        x = example_leaves();
        printf("done (%d)\n\n",x);

        if(fork())
                wait(0);
        else {
                example_searches(file);
                exit(0);
        }

        //print_tree(1,example);

        y = getrusage(RUSAGE_CHILDREN, &usage);
        if(y)
                perror("getrusage()");
        else{
                duration = 0;
                duration = (float)usage.ru_utime.tv_usec;
                duration /= (float)1000000.0;
                duration += (float)usage.ru_utime.tv_sec;


                printf("\nDuration: %0.4f\n",duration);
                printf("\ntv_usec: %lu\n",usage.ru_utime.tv_usec);
        }

        return 0;
}
*/
#pragma GCC diagnostic pop
