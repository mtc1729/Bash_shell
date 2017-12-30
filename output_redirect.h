#ifndef OUT_REDIR_H
#define OUT_REDIR_H
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>

int call_output_redirect(int i,char *argv[][10],int start)
{
   // close(1); // Release fd no - 1
   int status;
    if(fork()==0)
    {
    int file_desc=open(argv[i+1][0], O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR); // Open a file with fd no = 1

    
    if(file_desc < 0)
    {
        return -1;
    }
    
     dup2(file_desc,1);       
     dup2(file_desc,2);       
     close(file_desc);
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
