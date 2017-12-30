#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void call_exit(){
	//kill(getppid(),SIGKILL);
	kill(0,SIGINT);
	signal(SIGINT,SIG_DFL);
	return;
}

	
		
