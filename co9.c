#include<stdio.h>
typedef struct{
    int w;
    int h;
}Rect;
int area(Rect *r){
    return (r->w*r->h);
}
Rect* bigger(Rect *r1,Rect *r2){
    if(r1->w*r1->h>r2->w*r2->h){
        return r1;
    }
    return r2;
}
int main(){
    Rect a={4,5};
    Rect b={6,7};
    area(&a);
    area(&b);
    bigger(&a,&b);
    printf("a面积%d\n",area(&a));
    printf("b面积%d\n",area(&b));
    Rect *p = bigger(&a,&b);
    printf("更大矩形 w=%d h=%d\n",p->w,p->h);
    return 0;
}