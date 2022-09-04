#include<sys/stat.h>  //stat
#include <stdio.h>

#include <sys/types.h>//dir
#include <dirent.h>//dir

#include <string.h>

#include<unistd.h>//getcwd

int main(int argc,char* argv[]){
    int isDetail=0;
    char fileDirect[200]="";
    int opt_l=-1;
    // -l 参数检查
    int i;
    for (i=1; i < argc; i++){
        if(strcmp("-l",argv[i])==0 ){
            isDetail=1;
            opt_l=i;
        }
    }

    if(argc < 2){//myls
        getcwd(fileDirect,200);
    
    }else if(argc == 2){
        if(isDetail){//mysl -l;
            getcwd(fileDirect,200);
        }else{//mysl 路径;
            strcat(fileDirect,argv[1]); 
        }
    }else if(argc==3){// mysl -l 路径
        strcat(fileDirect,argv[3-opt_l]); 
    }
    
    
    struct stat st;
    struct dirent * dirent;
    DIR* dir = opendir(fileDirect);//开启流dir
    while( (dirent = readdir(dir))!=NULL ){
        char name[100] = "";
        strcat(name,&dirent->d_name);
        printf("%s ",name);
        if(isDetail){//打印出文件所有者 
            char sp[]="/";
            strcat(fileDirect,sp);
            strcat(fileDirect,name);
            stat(fileDirect,&st);
            printf("uid:%d\n",(int)st.st_uid);
            printf("gid:%d\n",(int)st.st_gid);
        }
    }
    
    closedir(dir);
    return 0;

}