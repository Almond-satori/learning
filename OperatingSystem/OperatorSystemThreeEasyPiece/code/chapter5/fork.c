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
        printf("child before [pid:%d] x=%d\n",(int)getpid(),x);
        x = 10;
        printf("child after [pid:%d] x=%d\n",(int)getpid(),x);
    }else { //父进程
        printf("parent before [pid:%d] x=%d\n",(int)getpid(),x);
        x = 200;
        printf("parent after [pid:%d] x=%d\n",(int)getpid(),x);
    }
}
