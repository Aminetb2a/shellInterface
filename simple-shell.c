/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

 //Project one
 // Ahmed Amine TALEB BAHMED
 //130201120

 #include <stdio.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <string.h>

 #include  <signal.h>
 #include  <sys/ipc.h>
 #include  <sys/shm.h>



 #define MAX_LINE 80 /* The maximum length command */
 #define HISTORY_SIZE 10 // history
 #define BUFFER_SIZE 50
 #define buffer "\n\ History:\n"

 char hist[HISTORY_SIZE][BUFFER_SIZE];
 int cnt = 0;

 void printHist();
 int cmd(char input[], char *args[],int *flag,int *run);

 int main(void)
 {
     char input[MAX_LINE];
     int flag;
     char *args[MAX_LINE/2 + 1]; /* command line arguments */
     int should_run =1;          /* flag to determine when to exit program */

     pid_t pid,tpid;
     int i;


     while (should_run)
     {
         flag = 0;
         printf("osh>");
         fflush(stdout);

         if(-1!=cmd(input,args,&flag,&should_run))
         {
           pid = fork();

           if (pid < 0)
         	{

             		printf("Fork failed.\n");
             		exit (1);
         	}

        		else if (pid == 0)
         	{

             		if (execvp(args[0], args) == -1 && should_run != 0)
                 		printf("Error executing command\n");

                 else if (should_run == 0)
                 {
                   printf("terminated \n" );
                   kill (pid, SIGQUIT);
                   exit(-1);
                 }
        		 }

         	else
         	{
             		i++;
                 if (flag == 0)
            		 {
                 		i++;
                 		wait(NULL);
            		 }
         	}
         }
      }
 }

 void printHist()
 {

     printf("History:\n");

     int i;
     int j = 0;
     int histCnt = cnt;

     for (i = 0; i<HISTORY_SIZE;i++)
     {
         printf("%d.  ", histCnt);
         while (hist[i][j] != '\n' && hist[i][j] != '\0')
         {
             printf("%c", hist[i][j]);
             j++;
         }
         printf("\n");
         j = 0;
         histCnt--;
         if (histCnt ==  0)
             break;
     }
     printf("\n");
 }


 int cmd(char input[], char *args[],int *flag,int *run)
 {
    	int length;
     	int i;
     	int next_cmd;
     	int next_prm = 0;


  	length = read(STDIN_FILENO, input, MAX_LINE);


     next_cmd = -1;
     if (length == 0)
     exit(0);

     if (length < 0)
     exit(-1);

     for (i=0;i<length;i++)
     {
         switch (input[i])
         {
             case ' ':
             case '\t' :
                 if(next_cmd != -1)
                 {
                     args[next_prm] = &input[next_cmd];
                     next_prm++;
                 }
                 input[i] = '\0';
                 next_cmd = -1;
                 break;

             case '\n':
                 if (next_cmd != -1)
                 {
                     args[next_prm] = &input[next_cmd];
                     next_prm++;
                 }
                 input[i] = '\0';
                 args[next_prm] = NULL;
                 break;

             default :
                 if (next_cmd == -1)
                     next_cmd = i;
                 if (input[i] == '&')
                 {
                     *flag  = 1;
                     input[i] = '\0';
                 }
         }
     }

     args[next_prm] = NULL;

 if(strcmp(args[0],"history")==0)
         {
                if(cnt>0)
                 printHist();

                 else
                 printf("\nNo Commands in the history\n");

                 return -1;
         }
   else if(strcmp (args[0],"exit") == 0){ *run = 0; }
 	else if (args[0][0]-'!' ==0)
 	{
     int x = args[0][1]- '0';
 		int z = args[0][2]- '0';

 		if(x>cnt)
 		{
 		printf("\nNo Such Command in the history\n");
 		strcpy(input,"Wrong command");
 		}
 		else if (z!=-48)
 		{
 		printf("\nNo Such Command in the history. Enter <=!9 (buffer size is 10 along with current command)\n");
 		strcpy(input,"Wrong command");
 		}
 		else
 		{
 			if(x==-15)
 			{	 strcpy(input,hist[0]); }

       else if(x==0)
 			{	 printf("Enter proper command");
 				strcpy(input,"Wrong command");
 			}

 			else if(x>=1)
 			{
 				strcpy(input,hist[cnt-x]);

 			}

 		}
 	}
  for (i = 9;i>0; i--)
        	strcpy(hist[i], hist[i-1]);

     strcpy(hist[0],input);
     cnt++;
 	if(cnt>10)
 	{ cnt=10;
 	}
 }
