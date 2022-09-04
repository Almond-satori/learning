#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

int main(){
    int n = 0;
    struct timeval tv1;
    struct timeval tv2;
    char buffer[1];
    int fd = open("./testFile.txt",O_RDONLY);
    if(fd == -1){
        printf("can not open file\n");
        exit(-1);
    }
    gettimeofday(&tv1,NULL);    
    while(n<100){
        read(fd,buffer,0);
        n++;
    }
    gettimeofday(&tv2,NULL);
    long ans = tv2.tv_usec - tv1.tv_usec;
    printf("系统调用耗时结果是%d\n",ans);
    float avg = ((float)ans)/n;
    printf("平均系统调用耗时结果是%f\n",avg);
    close(fd);
}
/*
系统调用耗时结果是50
平均系统调用耗时结果是0.500000
*/