#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
void alloc_buf(uint8_t **p,int size){
    *p=(uint8_t*)malloc(size);
}
int main(){
    uint8_t *cb=NULL;
    alloc_buf(&cb,10);
    if(cb!=NULL){
        cb[1]=0X11;
    }
    printf("%#X",cb[1]);
    free(cb);
    getchar();
    return 0;
}