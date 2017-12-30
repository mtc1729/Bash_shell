#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int call_cd(char *argv){
	char path[256];
	if(chdir(argv)==-1){
   		printf("unknown directory : %s\n",argv);
   		return -1;
	}
	else
   		printf("%s\n",getcwd(path,sizeof(path)));
	return 0;
}

