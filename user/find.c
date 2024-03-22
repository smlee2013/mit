#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

char* fmtname(char* path){

	static char buf[DIRSIZ+1];
	char* p;
	
	memset(buf, 0, sizeof(buf));
	for(p = path + strlen(path); p >= path && *p != '/'; p--);
	++p;
	
	return p;
	
}

int norecurse(char* path){
	char* buf = fmtname(path);
	if(buf[0] == '.' && buf[1] == 0) 
		return 1;
	if(buf[0] == '.' && buf[1] == '.' && buf[2] == 0) 
		return 1;
	return 0;
}

void find(char* path, char* filename){
	
	// definations
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;
	
	
	if((fd = open(path, O_RDONLY)) < 0){
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}
	
	if(fstat(fd, &st) < 0){
		fprintf(2, "find: cannot stat %s\n", path);
		close(fd);
		return;
	}
	
	if(strcmp(fmtname(path), filename) == 0){
		printf("%s\n", path);
	}

	switch(st.type){
	
	case T_FILE:
		break;
		
	case T_DIR:
		// memset(buf, 0, sizeof(buf));
		strcpy(buf, path);
		p = buf + strlen(buf);
		*p ++ = '/';
		while(read(fd, &de, sizeof(de)) == sizeof(de)){
			if(de.inum == 0 || strcmp(de.name, ".") == 0 ||
			strcmp(de.name, "..") == 0)
				continue;
			memcpy(p, de.name, DIRSIZ);
			p[DIRSIZ] = 0;
			
			find(buf, filename);
		}
		break;
	}
	close(fd);
}

int main(int argc, char* argv[]){
	
	if(argc != 3 && argc != 2){
		fprintf(2, "usage: find [path] [filename]\n");
		exit(1);
	}
	
	if(argc == 2){
		find(".", argv[1]);
	}
	
	find(argv[1], argv[2]);
	exit(0);

}

