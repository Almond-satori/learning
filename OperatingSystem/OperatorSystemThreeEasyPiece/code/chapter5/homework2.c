#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main(int argc,char *argv[])
{
    int x = 0;
    printf("[pid:%d] x=%d\n",(int)getpid(),x);
    int rc = fork();
    int fd = open("./file",O_CREAT|O_WRONLY|O_TRUNC);

    if( rc < 0 ){
        printf("error!");
        exit(1);
    }else if(rc == 0){ //child process
        printf( "child before [pid:%d] fd = %d\n",(int)getpid(),fd);
        char *s1 = "child process string";
        write(fd,s1,strlen(s1));
    }else { //parent process
        // wait(NULL);// let child process write early
        char *s2 = "the string of parent process";
        printf( "parent before [pid:%d] fd = %d\n",(int)getpid(),fd);
        write(fd,s2,strlen(s2));
    }
    close(fd);
    /*
    result:
    1.parent and child can get fd value,and the value is same.
        [pid:22510] x=0
        parent before [pid:22510] fd = 3
        child before [pid:22511] fd = 3
    2.the details in "file" is:
        child process string   
    the later process will override the details that early process wrote.
    if we let child process write early,the string in "file" is:
        the string of parent process
    */
}