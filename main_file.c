#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h> //for file status description
#include <string.h>
#include <sys/types.h>
#include <dirent.h> //for readdir--ls command
#include <unistd.h>
#include <signal.h> // for exit
#include <sys/wait.h>
#include "cd.c"
#include "mkdir.c"
#include "rm.c"
#include "pwd.c"
#include "exit_using_c.c"
#include "history.c"
#include "header.h"

#define MAXLINE 1024
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int counting=0;

/*This code parses the line after encountering standard input/output (> , < ) or pipe operator (|) 
i.e. ls -l | sort | uniq
is parsed into tokens ls -l, sort and uniq
*/

int  parse(char *line, char **argv,char *operator){
	count = 0;
	int prev = -1,up = strlen(line),w,j,i;
	char *temp = line;
	for( i = 0; i < up; ++i){
		if((line[i] == '|' || line[i] == '<' || line[i] == '>' )){
			int flag = 0;
			for( j = prev+1; j < i; ++j){
				if(line[j] != '\0' && line[j] != ' ' && line[j] != '\n' && line[j] != '\t') flag = 1;
			}
			if(flag == 0){
				perror("Error: Invalid arugumments\n");
				return 0;
			}
			operator[count++] = line[i];
			line[i] = '\0';
			prev = i;
			*argv++ = temp;
			///	printf("parse %s %c\n",temp,operator[count-1]);
			temp = line+i;
			temp++;
		}
	}
	int flag = 0;
	for(j = prev+1; j < up; ++j){
		if(line[j] != '\0' && line[j] != ' ' && line[j] != '\n' && line[j] != '\t') flag = 1;
	}
	if(flag == 0){
		perror("Error: Invalid arugumments\n");
		return 0;
	}
	*argv++ = temp;
	//	printf("parse %s\n",temp);
	*argv = '\0';//mark the end of argument
	return 1;
}

/*This code parses each token obtained from parse() based on ' ' or '\t' or \n character i.e. ls -a -l is parsed into ls , -l, -a */
void parsing(char *input,char **argv){
	char *temp = input;
	char *new_var = input;
	counting = 0;
	while(*temp == ' ' || *temp == '\t') temp++;
	new_var = temp;
	while(*temp != '\0'){
		while(*temp != '\0' && *temp != ' ' && *temp != '\n' && *temp != '\t') temp++;
		while(*temp == '\n' || *temp == ' ' || *temp == '\t') *temp++ = '\0';
		*argv++ = new_var;
		counting++;
		//      printf("check2 %s %d ",new_var,counting);
		new_var = temp;
	}
	*argv = '\0';//mark the end of argument
}


int main(void){
	char buf[MAXLINE];//store the inbuilt commands
	pid_t pid;

	char val[500];
	int pipe_count=0,count_arg=0,shmid,shmid1,a,err_flag=0,background=0;
	char shell_path[40],str[100],call_count=0;//for the terminal
	char dir[40];//for chdir
	int status,i,w;//for making the parent process wait
	key_t key,key1;
	int flag=0,arg_count,count_pipe=0,counter=0;//for checking any matching command
	/*end with null */
	
	int count_sub=0;
	strcpy(shell_path,"New_shell$:~$ ");//the terminal representation
	//printf("%s",shell_path);
	// rl_bind_key('\t', rl_complete);
	if ((shmid = shmget(key, SHM_SIZE*200, 0644 | IPC_CREAT)) == -1) {
		perror("shmget");
		exit(1);
	}
	if ((shmid1 = shmget(key1, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
		perror("shmget");
		exit(1);
	}
	
	/* attach to the segment to get a pointer to it: */
	history_count = shmat(shmid, (void *)0, 0);
	//nChild = (int*)shmat(shmid1, 0, 0);
	history =(char**)shmat(shmid, 0, 0);
	prev =(int*)shmat(shmid1, 0, 0);
	//childpid=(int*)shmat(shmid1,0,0);
	if (history_count == (int *)(-1)) {
		perror("shmat");
		exit(1);
	}
	if (prev == (int *)(-1)) {
		perror("shmat");
		exit(1);
	}
	if (history == (char **)(-1)) {
		perror("shmat");
		exit(1);
	}


	history = malloc(sizeof(char**) * 200);
	for (a = 0; a < 200; a++) {
		history[a] = malloc(sizeof(char*) * SHM_SIZE);
	}
	do{
		if ( (pid = fork()) < 0)
		printf("fork error");
		else if (pid == 0) { /* child */
			//setting_environment();
			//do{
				//continue;
				err_flag=0;
				background=0;
				fflush(stdin);
				strcpy(shell_path,"New_shell$:~$ ");//the terminal representation
				printf("%s",shell_path);
				fgets(buf, MAXLINE, stdin);
				buf[strlen(buf) - 1] = 0;
				if(strcmp(buf,"\033[A") && strcmp(buf,"\033[B"))
				add_command_to_history(buf);
				flag=0;

				char *arg[sizeof(buf)];
				char operator[sizeof(buf)];

				//printf("%d\n",*history_count);
				if(parse(buf,arg,operator)){//parse the buffer into argument array
					char *arg_subset[count+1][10];
					arg[sizeof(buf)]=NULL;//terminate array with NULL:string
					for(arg_count=0;arg[arg_count]!=NULL;arg_count++){
						parsing(arg[arg_count],arg_subset[arg_count]);
						if(arg[arg_count+1]!=NULL && strcmp(arg_subset[arg_count][counting-1],"&")==0){
							printf("Error in command\n");
							err_flag=1;
							break;
						}
					}
					if(err_flag==1){
						exit(0);
					}
					if(strcmp(arg_subset[arg_count-1][counting-1],"&")==0){
						background=1;
						printf("%d\n",getpid());
						arg_subset[arg_count-1][counting-1]=NULL;
						 // close child's stdin
						fclose(0);
						fopen("/dev/null","r");//Redirect it to here
						
			                        

                                                
                                                
					}
					arg_subset[arg_count][counting]=0;
					if(count>0 ){
						if(fork()==0)
						{
							call_func(arg_subset,operator);
							exit(0);
						}
						else
						{

							wait(NULL);
							//exit(0);

						}
						//exit(0);
					}
					else {
						count_arg=0;
						if(strcmp(arg_subset[0][0],"cd")==0){ //present working directory
							call_cd(arg_subset[0][1]);
						}
						else if(strcmp(arg_subset[0][0],"mkdir")==0){ //present working directory
							if(fork()==0){
								call_mkdir(arg_subset[0]);
								exit(0);
							}
							else{
								wait(NULL);
								//exit(0);
							}
						}
						else if(strcmp(arg_subset[0][0],"ls")==0){
							if(fork()==0){
								if(execvp(arg_subset[0][0],arg_subset[0])==-1){
									perror("no such command found\n");
									exit(1);
								}
								exit(0);
							}
							else{
								wait(NULL);
								//exit(0);
							}
						}
						else if (strcmp(arg_subset[0][0],"rm")==0){
							if(fork()==0){
								call_rm(arg_subset[0]);
								exit(0);
							}
							else{
								wait(NULL);
								//exit(0);
							}
						}
						else if (strcmp(arg_subset[0][0],"pwd")==0){
							call_pwd();
						}
						else if (strcmp(arg_subset[0][0],"exit")==0){
							call_exit();
						}
						else if(!strcmp(arg_subset[0][0],"\033[A") || !strcmp(arg_subset[0][0],"\033[B")){

							up_down(arg_subset[0][0]);

						}
						else if (strcmp(arg_subset[0][0],"echo")==0){
							if(strcmp(arg_subset[0][1],"$HOME")==0){
								strcpy(val,getenv("HOME"));
								printf("%s\n",val);
							}
							else if(strcmp(arg_subset[0][1],"$USER")==0){
								strcpy(val,getenv("USER"));
								printf("%s\n",val);
							}
							else if(strcmp(arg_subset[0][1],"$PATH")==0){
								strcpy(val,getenv("PATH"));
								printf("%s\n",val);
							}
							else if(strcmp(arg_subset[0][1],"$TERM")==0){
								strcpy(val,getenv("TERM"));
								printf("%s\n",val);
							}
							else if(strcmp(arg_subset[0][1],"$DISPLAY")==0){
								strcpy(val,getenv("DISPLAY"));
								printf("%s\n",val);
							}
							else{
								if(fork()==0){
									if(execvp(arg_subset[0][0],arg_subset[0])==-1){
										perror("no such command found\n");
										exit(1);
									}
									exit(0);
								}
								else{
									wait(NULL);
									//exit(0);
								}
							}
						}
						else if (strcmp(arg_subset[0][0],"history")==0){
							for(i=0 ; i< *history_count;i++)
							printf("%s \n", history[i]);
						}
						
						else{
							if(fork()==0){
								if(execvp(arg_subset[0][0],arg_subset[0])==-1){
									perror("no such command found\n");
									exit(1);
								}
								exit(0);
							}
							else{
								wait(NULL);
								//exit(0);
							}
						}
                                           //     exit(0);
					}

					
				}
				//exit(0);
			//}while(1);
		}
		else if ( (pid = waitpid(pid, &status, 0)) < 0){
			printf("waitpid error");
			exit(1);
		}
		else if(pid>0){

			if (background==0) {
			
				w = waitpid(pid, &status,WNOHANG);
				

				//exit(0);
			}
			else{
				background=0;

	                      
                              


			}
		}
	}while(1);
	return(0);
}
