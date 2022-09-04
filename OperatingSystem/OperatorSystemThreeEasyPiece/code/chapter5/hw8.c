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
    int flag = -1;
    int fd[2]; 
    flag=pipe(fd);
    if(flag<0){
        printf("error flag\n");
        exit(-1);
    }

    int p1 = fork();
    if( p1 < 0 ){
        printf("error!");
        exit(1);
    }else if(p1 == 0){ //子进程
        close(fd[0]); //close read,becase we are going to write
        char *s1 = "data from p1";
        sleep(5);
        write(fd[1],s1,strlen(s1));// write s1 to fd[1]
        close(fd[1]); //close write,becase we have finished writing
        printf("p1 process over\n");
        return 0;
    }
    // wait until p1 finished
    // waitpid(p1,NULL,0); //wait process whose id equals p1

    int p2 = fork();
    if( p2 < 0 ){
        printf("error!");
        exit(1);
    }else if(p2 == 0){ //子进程
        printf("now is p2\n");
        close(fd[1]); // close write,becase we are going to read.
        char buf[1024];
        int size = read(fd[0],buf,1024);
        if( size < 0){
            printf("error while reading\n");
            exit(-1);
        }
        buf[size] = '\0';
        printf("p2 received successfully,the data is:\n");
        printf("%s\n",buf);

        return 0;
    }

    waitpid(p1,NULL,0);
    waitpid(p2,NULL,0);
    printf("parent process over\n");
    return 0;
    /*
    result:
        p1 process over
        now is p2
        p2 received successfully,the data is:
        data from p1
        parent process over
    */
}