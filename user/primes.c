#include "kernel/types.h"
#include "user.h"

void prime(int pipeRead){
    // printf("pid:%d\n",getpid());
    int readNum=1;
    if(read(pipeRead,&readNum, 4)){
        int saveNum=readNum;
        printf("prime %d\n",saveNum);
        int p[2];
        pipe(p); /*正常创建后，p[1]为管道写入端，p[0]为管道读出端*/ 
        int pid=fork();
        if(pid){     //父进程
            close(p[0]);    //关闭管道读端
            while (read(pipeRead,&readNum, 4)){
                if(readNum%saveNum){
                    write(p[1],&readNum,4);
                    // printf("%d: write %d\n",getpid(),readNum);
                }
            }
            close(pipeRead);//关闭父进程管道读端
            close(p[1]);    //关闭管道写端
            wait(0);
        }else{     //子进程
            close(p[1]);    //关闭管道写端
            prime(p[0]);
        }
    }
}

int main(int argc,char* argv[]){
    int p[2];
    pipe(p); /*正常创建后，p[1]为管道写入端，p[0]为管道读出端*/ 

    int pid=fork();
    if(pid){     //父进程
        close(p[0]);
        for(int i=2;i<=35;i++){
            write(p[1],&i,4);
        }
        close(p[1]);
        wait(0);
    }else{     //子进程
        close(p[1]);
        prime(p[0]);
        // close(p[1]);
    }

    exit(0);

}