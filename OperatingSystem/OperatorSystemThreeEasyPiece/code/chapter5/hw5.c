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
        int res = wait(NULL);
        printf("child res = %d\n",res);
        printf("child before [pid:%d] x=%d\n",(int)getpid(),x);
        x = 10;
        printf("child after [pid:%d] x=%d\n",(int)getpid(),x);
    }else { //父进程
        int res = wait(NULL);
        printf("res = %d\n",res);
        printf("parent before [pid:%d] x=%d\n",(int)getpid(),x);
        x = 200;
        printf("parent after [pid:%d] x=%d\n",(int)getpid(),x);
    }
    /*
    result: 
    1.wait will return the processId of the early process.
        [pid:27048] x=0
        child before [pid:27049] x=0
        child after [pid:27049] x=10
        res = 27049
        parent before [pid:27048] x=0
        parent after [pid:27048] x=200
    2.if we call wait() in both process:
        [pid:27424] x=0
        child res = -1
        child before [pid:27425] x=0
        child after [pid:27425] x=10
        res = 27425
        parent before [pid:27424] x=0
        parent after [pid:27424] x=200
    the pid in early process is -1.
    */
}
