#include "kernel/types.h"
//#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    if(argc != 2){
        fprintf(2, "arguments amount error!\n");
        exit(1);
    }
    printf("sleeping...\n");
    
    char* p = argv[1];
    int n = atoi(p);
    int ret = sleep(n);
    printf("over!\n");
    exit(ret);
}
