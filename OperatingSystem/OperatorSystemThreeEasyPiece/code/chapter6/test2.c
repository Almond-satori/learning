#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <string.h>

//测量切换上下文花费的时间成本 
int main(){
    int pipe1[2];//parent 向 child 写入
    int pipe2[2];//child 向 parent 写入
    int pi1 = pipe(pipe1);    
    int pi2 = pipe(pipe2);    
    if( pi1 < 0 || pi2 < 0){
        printf("pipe error\n");
        exit(-1);
    }
    char buf1[]="hellowrold";
    char buf2[]="hellowrold";
    //时间模块
    struct timeval tvStart;
    struct timeval tvEnd;
    int p1 = fork();
    if(p1 < 0){
        printf("process fork error\n");
        exit(-1);
    }
    int i;
    for(i = 0;i < 10;i++){
        //父子之间的start end 时间不能共享,需要将其传输
        if(p1 == 0){ //子进程

            read(pipe1[0],buf1,10);
            gettimeofday(&tvEnd,NULL);
            printf("time that parent switch to child is: %d ms\n",tvEnd.tv_usec - atol(buf1));
            gettimeofday(&tvStart,NULL);
            sprintf(buf1,"%d",tvStart.tv_usec);
            write(pipe2[1],buf1,10);
        }else{ //父进程

            gettimeofday(&tvStart,NULL);
            sprintf(buf2,"%d",tvStart.tv_usec);
            write(pipe1[1],buf2,strlen(buf2));//将开始时间写入子进程
            read(pipe2[0],buf2,10);
            gettimeofday(&tvEnd,NULL);
            printf("time that child switch to parent is: %d ms\n",tvEnd.tv_usec - atol(buf2));
            
        }
    }
    
}

/*
time that parent switch to child is: 83 ms
time that child switch to parent is: 34 ms
time that parent switch to child is: 36 ms
time that child switch to parent is: 35 ms
time that parent switch to child is: 36 ms
time that child switch to parent is: 41 ms
time that parent switch to child is: 29 ms
time that child switch to parent is: 27 ms
time that parent switch to child is: 20 ms
time that child switch to parent is: 38 ms
time that parent switch to child is: 26 ms
time that child switch to parent is: 25 ms
time that parent switch to child is: 34 ms
time that child switch to parent is: 26 ms
time that parent switch to child is: 24 ms
time that child switch to parent is: 27 ms
time that parent switch to child is: 27 ms
time that child switch to parent is: 26 ms
time that parent switch to child is: 69 ms
time that child switch to parent is: 31 ms
avg:34.7ms
*/