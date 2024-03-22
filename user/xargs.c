#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define MSGSIZE 16

int main(int argc, char* argv[]){
    // read each std output 
    char buf[MSGSIZE];
    read(0, buf, MSGSIZE);


    // as the xargs arguments
    char* xargv[MAXARG];
    int xargc = 0;
    
    for(int i = 1; i < argc; i++){
        // printf("参数%d是: %s\n", i, argv[i]);
        xargv[xargc] = argv[i];
        xargc++;
    }

    char* p = buf;
    for(int i = 0; i < MSGSIZE; i++){
        if(buf[i] == '\n'){
            int pid = fork();
            if(pid > 0){
                p = &buf[i+1];
                wait(0);
            }else{
                buf[i] = 0;
                xargv[xargc++] = p;
                xargv[xargc++] = 0;
                exec(xargv[0], xargv);
            }            
        }


    }

    wait(0);
    exit(0);
}
