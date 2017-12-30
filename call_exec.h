#ifndef CALL_EXEC_H
#define CALL_EXEC_H
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int  call_exec(char *argv[][10],int start){
	if(fork()==0){
		if(execvp(argv[start][0], argv[start])==-1) {
			perror("argument error");
			exit(1);
        	}
       		exit(0);
       	}
       	else{
        	wait(NULL);
        	//printf("New shell$:~$ ");
        	exit(0);  
       	}
}
#endif
