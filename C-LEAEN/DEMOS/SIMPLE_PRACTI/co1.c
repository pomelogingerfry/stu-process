#include<stdio.h>
int func(int arr[],int a){
    int i=1;
    for(i=1;i<5;i++){
        if(arr[i]%2==0){
            printf("%d",arr[i]);
            return arr[i];
        }
    }
    return 0;
}
      int (*cla)(int arr[],int n);
        int main(){
            int b[]={1,2,3,4,5};
            cla=func;
           int res=cla(b,5);
            printf("%d",res);
            getchar();
            return 0;
        }


