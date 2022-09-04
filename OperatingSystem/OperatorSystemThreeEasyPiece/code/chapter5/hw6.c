#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char *argv[])
{
    int x = 0;
    printf("[pid:%d] x=%d\n",(int)getpid(),x);
    int rc = fork();
    if( rc < 0 ){
        printf("error!");
        exit(1);
    }else if(rc == 0){ //子进程
        int res = waitpid(-1,NULL,WNOHANG);//pid=-1 等待任何子进程, 相当于wait(),WNOHANG 如果没有任何已经结束的子进程则马上返回, 不予以等待.
        printf("child res = %d\n",res);
        printf("child before [pid:%d] x=%d\n",(int)getpid(),x);
        x = 10;
        printf("child after [pid:%d] x=%d\n",(int)getpid(),x);
    }else { //父进程
        int res = waitpid(-1,NULL,WNOHANG);
        printf("res = %d\n",res);
        printf("parent before [pid:%d] x=%d\n",(int)getpid(),x);
        x = 200;
        printf("parent after [pid:%d] x=%d\n",(int)getpid(),x);
    }
    /*
    result: 
        res = 0
        parent before [pid:28771] x=0
        parent after [pid:28771] x=200
        child res = -1
        child before [pid:28772] x=0
        child after [pid:28772] x=10

    */
}
