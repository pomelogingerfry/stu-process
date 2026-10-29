#include<stdio.h>

int func(int arr[],int a){
    int i=0;
    for(i=0;i<a;i++){
        if(arr[i]%2==0){
            return arr[i];
        }
    }
    return 0;
}
void run(int (*p)(int[],int),int arr[],int len){
    int r=p(arr,len);
    printf("%d",r);
}
int main(){
    int a[]={11,34,56,7,8,9};
    run(func,a,6);
    getchar();
    return 0;
}