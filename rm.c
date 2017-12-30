#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdarg.h>
void call_rm(char*list[]){ 
	execvp(list[0],(char* const*)list);
  	return;
}


