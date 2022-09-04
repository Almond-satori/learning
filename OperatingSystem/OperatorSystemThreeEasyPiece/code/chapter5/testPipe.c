#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// parent read->fd[0]
// parent write->fd[1]
// child read->fd[0]
// child write->fd[0]
//if we want to transfer data from parent to child,we should close parent's read and child's write.

int main(int argc,char *argv[])
{
    int fd[2];
    pipe(fd);
    int p1 = fork();

    if(p1==0){//child
        printf("child\n");

        close(fd[1]);//1&3.close child write
        char buf[1024];
        int len = read(fd[0],buf,1024);
        buf[len] = '\0';
        printf("%s\n",buf);
        memset(buf,0,1024);
        //2&3.twice read:
        len = read(fd[0],buf,1024);
        buf[len] = '\0';
        printf("%s\n",buf);
        printf("child over\n");

        return 0;
    }else{//parent
        printf("parent\n");
        close(fd[0]);
        char buf[] = "hello world this is thd data of the parent process";
        write(fd[1],buf,strlen(buf));
        close(fd[1]);
        printf("parent over\n");
        return 0;
    }

    /*
    1.read once,didn't close child write:
        parent
        parent over
        child
        hello world this is thd data of the parent process
        child over

    2.read twice,didn't close child write:(child can not over)
        parent
        parent over
        child
        hello world this is thd data of the parent process

    3.read twice,close child write:(child end normally)
        parent
        parent over
        child
        hello world this is thd data of the parent process

        child over

    */
}