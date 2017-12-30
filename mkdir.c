#include <sys/types.h>
#include <fcntl.h>       
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>

void  call_mkdir(char *list[])
{
         
        if(execvp(list[0],list)==-1)
        {
        
                perror("No directory created\n");
        }
        
        
  return ;

 }

/*int main(int argc,char*argv[]){

int i;
char**list;

list=(char**)calloc(argc+2,sizeof(char*));

list[0]="/bin/mkdir";
list[1]="-v";

for(i=0;i<argc-1;i++){
	*(list+i+2)=(char*)calloc(strlen(argv[i+1]),sizeof(char));
	*(list+i+2)=argv[i+1];
}

call_mkdir(list);


	return 0;
}*/
