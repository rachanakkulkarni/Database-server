/*htable.c*/
#include "htable.h"

Hash hash_naive(int8 *input, int16 size){
        Hash hash;
        int16 x;
        int8 *p;

        hash.hash = 0;
        for(p = input, x = 0; x<size; p++, x++){
                hash.hash += *p;
        }

        return hash;
}


int main(int argc, char *argv[]) {
        int8 *str;
        int16 size;
        Hash h;

        if(argc<2) {
                fprintf(stderr, "Usage: %s STRING", *argv);
                return -1;
        }

        str = (int8*)argv[1];
        size = (int16)strlen((char *)str);

        h =  hash(str, size);
        printf("0x%.05x\n", (unsigned int)h.hash);

        return 0;
}
