#include <stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include "pipe.h" //for function call_pipe, handles various combination of standard input-output and pipe

void call_func(char *argv[][10],char *operator)
{

    pid_t p;

    int in,i,count_pipe=0,input_red=-1,output_red=-1,flag=0,x;
    int status;

    char str[300];

    //printf("%d\n",count);
    for(i=0;i<count;i++)
    {

        if(operator[i]=='|')
        {

              count_pipe++;  
              
           
       }
      else
         break;
            /*if(operator[i]=='>')
            {
                call_input_redirect(i,argv);
      
            }   
            
/*            if(strcmp(argv[i][0],"cat")==0)
            {
            
                execvp(argv[i][0],argv[i]);
            }         
            if(strcmp(argv[i][0],"ls")==0)
            {
            
                execvp(argv[i][0],argv[i]);
            }         */

       
    }  
               if(count_pipe>0)
            {
              
               if(i<count)
                 call_pipe(argv,0,count_pipe,operator,i);
               else  
                 call_pipe(argv,0,count_pipe,operator,-1);

 
             }
          
           
            else if(count_pipe==0)
            { 
               i=0;
               while(i<count)
               {
                   if( operator[i]=='<' )
                   {
                           
                                
                                 i++;
                                 input_red=i-1;
                                 printf("%d\n",i);
                   
                   }
                   else
                   {
                     break;
                    } 
                     
                } 
                
              
                while(i<count)    
                {
                
                    if( operator[i]=='>' )
                   {
                                  flag=1;   
                                  output_red=i;                      
                                  i++;
                   
                   }
                   else
                   {

                    
                     break;
                    }
                   

                
                }
                flag=0;

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
//            printf("heghwkr");
}
