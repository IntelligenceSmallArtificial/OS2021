#include "kernel/types.h"
#include "user.h"

int main(int argc,char* argv[]){
    int p1[2];
    int p2[2];
    pipe(p1); /*正常创建后，p[1]为管道写入端，p[0]为管道读出端*/ 
    pipe(p2);


    int pid=fork();
    if(pid){     //父进程
        close(p1[0]);
        close(p2[1]);
        write(p1[1],"ping",4);
        // char readStr1="";   //错误
        // char readStr1;      //错误
        char readStr1[64];   //正确，长度需要足够
        read(p2[0], readStr1, 4);
        printf("%d: received %s\n",getpid(),readStr1);
        close(p1[1]);
        close(p2[0]);

    }else{     //子进程
        close(p1[1]); 
        close(p2[0]); 
        // char readStr2="";   //错误
        // char readStr2;      //错误
        char readStr2[64];   //正确，长度需要足够
        read(p1[0], readStr2, 4);
        printf("%d: received %s\n",getpid(),readStr2);
        write(p2[1],"pong",4);
        close(p1[0]);
        close(p2[1]);

    }

    exit(0);

}
