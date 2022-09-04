#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[]){

    if(argc != 3){
        printf("error input:%s,%s",argv[1],argv[2]);
        exit(-1);
    }
    //创建计数器
    struct timeval start,end;
    //argv[1] num, argv[2] tryTimes
    int num = atoi(argv[1]); //num为数组占页面的个数指数级递增
    int tryTimes = atoi(argv[2]); //访问次数,固定值
    int pageSize = getpagesize(); //获取内存页大小
    int jump = pageSize/sizeof(int);//让每次访问都要去内存
    int a[num*jump];
    // printf("pagesize:%d \n",pageSize);
    // printf("jump:%d \n",jump);

    memset(a, 0, num * pageSize);//因为第一次访问数组会很慢，可以先对其进行赋值
    
    
    gettimeofday(&start,NULL);
    for (int j = 0; j < tryTimes; j++)
        for (int i = 0; i < num*jump; i+=jump)
            a[i]+=1;
    gettimeofday(&end,NULL);

    printf("tryTimes: %d,once access memory use time: %f Microseconds(us)\n",tryTimes,
    ((double)end.tv_usec - (double)start.tv_usec)/tryTimes/num);

    return 0;
}