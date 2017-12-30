#ifndef IN_OUT_RED_H
#define IN_OUT_RED_H
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int  call_input_output_redirect(int op_out,int op_in,char *argv[][10],int start)
{
   // close(1); // Release fd no - 1
   int status,in,out;
    if(fork()==0)
    {
          // printf("%s,%s,%s\n",argv[0][0],argv[op_in+1][0],argv[op_out+1][0]);
           in = open(argv[op_in+1][0],O_RDONLY);
            if(in < 0)
            {
                        return -1;
            }
           dup2(in,0);
           close(in);
           out = open(argv[op_out+1][0],O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR); // Should also be symbolic values for access rights
           if(out < 0)
            {
                        return -1;
            }
           dup2(out,1);
           close(out);
           
           execvp(argv[start][0],argv[start]);
           exit(0);
   }
      else
   {
     wait(NULL);
     exit(0);
   }

   
    // execvp(argv[i][0],argv[i]); // By default, the program writes to stdout (fd no - 1). ie, in this case, the file
     //exit(0);
    return 0;
}
#endif
