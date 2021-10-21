#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"



int main(int argc, char *argv[]){
    if(argc==0) {
        printf("xargs input error!\n");
        exit(0);
    }
    char in[648];               //标准输入流总字符串
    char outputArgv[MAXARG][64];    //整合后的参数
    int outputArgc=0;             //整合后的参数数量
    int argLength=0;            //当前参数长度
    int readLength;
    char *p[MAXARG];
    while((readLength = read(0, in, sizeof(in)))>0){
        outputArgc=0;
        argLength=0;
        for(int j=1;j<argc;j++){
            strcpy(outputArgv[outputArgc],argv[j]);
            p[outputArgc]=outputArgv[outputArgc];
            outputArgc++;
        }
        for (int i = 0; i < readLength; i++){
            if(in[i]==' '){
                if(argLength>0){
                    outputArgv[outputArgc][argLength]='\0';
                    p[outputArgc]=outputArgv[outputArgc];
                    argLength=0;
                    outputArgc++;
                }
            } else if(in[i]=='\n'){
                if(argLength>0){
                    outputArgv[outputArgc][argLength]='\0';
                    p[outputArgc]=outputArgv[outputArgc];
                    argLength=0;
                    outputArgc++;
                }
                p[outputArgc]=0;
                if(fork()){     //父进程
                    wait(0);
                    outputArgc=0;
                    argLength=0;
                    for(int j=1;j<argc;j++){
                        strcpy(outputArgv[outputArgc],argv[j]);
                        p[outputArgc]=outputArgv[outputArgc];
                        outputArgc++;
                    }
                }else{
                    exec(argv[1],p);
                    wait(0);
                    exit(0);
                }
            } else if(in[i]=='\0'){
                break;
            } else{
                outputArgv[outputArgc][argLength++]=in[i];
            }
        }
    }
    exit(0);
}
