#include<sys/stat.h>  
#include <stdio.h>

int main (int argc,char* argv[]){
    if(argc < 2){
        printf("error inpute \n");
        exit(-1);
    }
    struct stat st;
    stat(argv[1],&st);
    printf("details:\n");
    printf("size: %d B\n",(int)st.st_size);
    printf("number of allocated disk blocks: %d\n",(int)st.st_blocks);
    printf("number of hard links : %d\n",(int)st.st_nlink);
    return 0;
}