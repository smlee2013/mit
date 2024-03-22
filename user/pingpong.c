#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1

int main(int argc, char* argv[]){
	int p2c[2];
	int c2p[2];
	
	pipe(p2c);
	pipe(c2p);
	
	char p = 'p';
	int exit_status = 0;
	
	if(fork() == 0){	//child
		close(p2c[WR]);
		close(c2p[RD]);
		
		if(read(p2c[RD], &p, sizeof(char)) != sizeof(char)){
			fprintf(2, "child reads failed\n");
			exit_status = 1;
		}else{
			printf("%d: received ping\n", getpid());
			if(write(c2p[WR], &p, sizeof(char)) != sizeof(char))
				fprintf(2, "child writes failed\n");
		}
		
		exit(exit_status);
	}else{			//parent
		close(p2c[RD]);
		close(c2p[WR]);
		
		if(write(p2c[WR], &p, sizeof(char)) != sizeof(char)){
			fprintf(2, "parent writes failed\n");
			exit_status = 1;
		}else{
			if(read(c2p[RD], &p, sizeof(char)) != sizeof(char)){
				fprintf(2, "parents reads failed\n");
				exit_status = 1;
			}else{
				printf("%d: received pong\n", getpid());
			}
		}
		
		exit(exit_status);
	}
}
