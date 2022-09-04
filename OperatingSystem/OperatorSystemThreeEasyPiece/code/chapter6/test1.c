#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

int main(){
    int n = 0;
    int totalTime = 0;
    struct timeval tv1;
    struct timeval tv2;
    char buffer[1];
    int fd = open("./testFile.txt",O_RDONLY);
    if(fd == -1){
        printf("can not open file\n");
        exit(-1);
    }
    while(n<100){
        gettimeofday(&tv1,NULL);    
        read(fd,buffer,0);
        gettimeofday(&tv2,NULL);
        long ans = tv2.tv_usec - tv1.tv_usec;
        printf("耗时是 %d ms\n",ans);
        totalTime+=ans;
        n++;
    }
    close(fd);
    printf("系统调用耗时结果是%d\n",totalTime);
    printf("平均系统调用耗时结果是%f\n",((float)totalTime)/n);
}
/*
系统调用耗时结果是74
平均系统调用耗时结果是0.740000
*/