#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path,char *needName){
  int fd;             //文件描述符
  struct dirent de;   //定义文件名变量
  struct stat st;     //定义状态变量

  if((fd = open(path, 0)) < 0){         //未打开
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){             //读取状态失败
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
    case T_FILE:
      break;

    case T_DIR:
        // printf("find: receive dir path %s\n",path);
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0) continue;
            // printf("find: %s in %s\n",de.name,path);
            if(strcmp(de.name,".")==0||strcmp(de.name,"..")==0){
                continue;
            }
            char currentPath[512];
            int i = 0;                  //生成子文件路径
            for (;path[i]; i++){
                currentPath[i]=path[i];
            }
            currentPath[i++]='/';
            int j = 0;
            for (;de.name[j]; i++,j++){
                currentPath[i]=de.name[j];
            }
            currentPath[i]='\0';
            
            if(strcmp(de.name,needName)==0){    //输出判断
                printf("%s\n",currentPath);
            }
            find(currentPath,needName);
        }
        break;
  }
  close(fd);
}

int main(int argc,char* argv[]){
  if(argc == 3){
    find(argv[1], argv[2]);
  }else{
    printf("input error!\n");
  }
  exit(0);
}
