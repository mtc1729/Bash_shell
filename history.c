#include<stdio.h>
#include<string.h>
#define HISTORY_MAX_SIZE 100

char **history;
int *history_count = 0;
int *prev=0;

void add_command_to_history(  char *command ){
   unsigned int index,k;
   	if ((*history_count) < HISTORY_MAX_SIZE) {
    	history[(*history_count)++] = strdup(command) ;
   	}else {
       	//free( history[0] );
        for (index = 1; index < HISTORY_MAX_SIZE; index++) {
             history[index - 1]=history[index];
        }
        history[HISTORY_MAX_SIZE - 1] = strdup( command );
       
    }
    k=*history_count;
    *prev=k;
	return;
}
void up_down(char *input){
  int k;
	if(!strcmp(input,"\033[A")){
    
		if(*history_count>0){
     
      k=*prev;
			printf("%s\n",history[k-1]);//*prev-1]);

			k=k-1;
      *prev=k;

		}
	}
    if(!strcmp(input,"\033[B")){

		if(*prev<*history_count-1){
			printf("%s\n",history[*prev+1]);
			(*prev)++;
     }                
	}
	return;
}

