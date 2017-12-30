#include<unistd.h>
#include<stdio.h>
void call_pwd(){
	char path[900];
  	getcwd(path,sizeof(path));
  	printf("%s\n",path);
  	return;
}

