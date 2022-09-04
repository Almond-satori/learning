#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc,char *argv[])
{
    int x = 0;
    printf("[pid:%d] x=%d\n",(int)getpid(),x);
    int rc = fork();
    if( rc < 0 ){
        printf("error!");
        exit(1);
    }else if(rc == 0){ //子进程
        printf("child [pid:%d] x=%d\n",(int)getpid(),x);
        char *myargs[3];
        myargs[0]=strdup("ls");
        myargs[1]=strdup("/"); // first arg in command
        myargs[2]=NULL;
        execvp(myargs[0],myargs);
        printf("this shouldn't print out\n");
    }else { //父进程
        printf("parent before [pid:%d] x=%d\n",(int)getpid(),x);
        x = 200;
        printf("parent after [pid:%d] x=%d\n",(int)getpid(),x);
    }
    /*
    result:
        [pid:26471] x=0
        parent before [pid:26471] x=0
        parent after [pid:26471] x=200
        child [pid:26472] x=0
        bin   dev   home.milan.tiger.txt  lost+found  newdisk  root  shellcode	tmp	      var
        boot  etc   lib			  media       opt      run   srv	userSoftware  www
        data  home  lib64		  mnt	      proc     sbin  sys	usr
    
    */
}