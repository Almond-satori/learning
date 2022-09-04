#include<sys/stat.h>  
#include <stdio.h>

#include<string.h>

#include <sys/types.h>

#include <sys/fcntl.h>
#include<unistd.h>
int main(int argc,char*argv[]){
    //mytail -n file
    //get option 
    if(argc < 3){
        printf("error inpute \n");
        exit(-1);
    }
    char file[200]="";
    strcat(file,argv[2]);
    int option_n = atoi(&argv[1][1]);

    //open file
    struct stat st;
    // stat(file,&st);
    int fd = open(file,O_RDONLY);
    int lines = 1;
    char buf[1025];
    int size;
    while( size=read(fd,buf,1024) ){
        int i;
        for (i = 0; buf[i]&&i<size; i++)//i<size,否则将读完本次buf部分会继续读入上一次的buf 
        {
            if(buf[i]=='\n'){
                lines++;
            }
        }
    }
    lines=lines-option_n+1;
    lseek(fd,0,SEEK_SET);//return to the top
    int l = 1;
    while( size=read(fd,buf,1024) ){
        int i;
        for (i = 0; buf[i] && (i<size); i++)
        {
            if(buf[i]=='\n'){
                l++;
            }
            if(l>=lines){
                putchar(buf[i]);
            }
        }
    }
    return 0;
}