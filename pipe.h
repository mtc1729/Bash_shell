#ifndef PIPE_H
#define PIPE_H
#include <stdio.h>
#include<unistd.h>
#include<fcntl.h>
int call_pipe(char *argv[][10],int start,int count_pipe,char *operator1,char op_pos)
{
    int *pipe_fds,i,input_red=-1,output_red=-1,x;
    pipe_fds=(int*)malloc(2*count_pipe*sizeof(int));
    pid_t pid;

    int stdin_save, stdout_save;
    int status;

    char str[300];
    int fp,j;
    for(i=0;i<count_pipe;i++)
    {
        if (pipe(pipe_fds+2*i) < 0)
                return -1;
    }
    


while( start<=count_pipe ){
    pid = fork();
//    printf("%d,%d\n",start,count_pipe);
    if( pid == 0 ){
        /* child gets input from the previous command,
            if it's not the first command */
        if( start>0 ){
            if( dup2(pipe_fds[(start-1)*2],0) < 0){
                perror("pipe error");
                exit(1);
            }
        }
        /* child outputs to next command, if it's not
            the last command */
        if( start<count_pipe ){
            if( dup2(pipe_fds[start*2+1],1) < 0 ){
                perror("pipe error");
                exit(1);
            }
        }
        for(j=0;j<(2*count_pipe);j++)
       {
             close(pipe_fds[j]);
                
       } 
        if(start==count_pipe && op_pos!=-1)
        {
               i=start;
               
            
               while(i<count)
               {
                   if( operator1[i]=='<' )
                   {
                           
                                
                                 i++;
                                 input_red=i-1;
            
                   
                   }
                   else
                   {
                     break;
                    } 
                     
                } 
            
              
                while(i<count)    
                {
            
                
                    if( operator1[i]=='>' )
                   {
                                
                                  output_red=i;                      
            
                                  i++;
                   
                   }
                   else
                   {

                    
                     break;
                    }
                   

                
                }
            
             
  
            //    call_input_output_redirect(i,argv);
 //      printf("output=%d, input=%d\n",output_red,input_red);
            if(i==count)
            {
                if(output_red>-1 && input_red==-1)
                {

                        x=call_output_redirect(output_red,argv,0);
                        if(x==-1)
                        {
                                perror("No such file\n");
                        }
      
                }  
                else if(output_red==-1 && input_red>-1)
                {
                        x=call_input_redirect(input_red,argv,0);
                         if(x==-1)
                        {
                                perror("No such file\n");
                        }
           
                }
                else if(output_red>-1 && input_red>-1)
                {
                       
                        x=call_input_output_redirect(output_red,input_red,argv,0);
                         if(x==-1)
                        {
                                perror("No such file\n");
                        }
      
                }
            }  
            else 
            {
                printf("Unjustified combination\n");
            }
             
 }
       else
       {
             call_exec(argv,start);
            
       } 
        
        exit(0);
    } else if( pid < 0 ){
        perror("process error");
                exit(1);
    }
       
    start++;
}
  
wait(NULL);
for(i=0;i<(2*count_pipe);i++)
      {
             close(pipe_fds[i]);
                          
      }

//exit(0); 


   return 0;   
}
#endif
