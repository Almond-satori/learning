#include <sys/stat.h>  
#include <stdio.h>
#include <string.h>
#include <sys/types.h>//dir
#include <dirent.h>//dir

#include<fcntl.h>
#include<unistd.h>


void readTotal(char filedir[]){
    DIR* dir = opendir(filedir);//开启流dir
    struct dirent * dirent;
    while( (dirent = readdir(dir))!=NULL ){
        char dname[100]="";
        strcpy(dname,dirent->d_name);
        if(strcmp(dname,".")==0||strcmp(dname,"..")==0)
            continue;
        printf("%s ",dname);

        char subfiledir[200];
        strcpy(subfiledir,filedir);
        strcat(subfiledir,"/");
        strcat(subfiledir,dirent->d_name);
        
        struct stat st;
        stat(filedir,&st);
        if( S_ISDIR(st.st_mode) )//如果该文件是目录，打印子目录,?
			readTotal(subfiledir);
    }
    closedir(dir);
}

int main(int agrc,char* argv[]){
    //find  目录
    char filedir[200]="";
    strcat(filedir,argv[1]);
    readTotal(filedir);
    return 0;
}