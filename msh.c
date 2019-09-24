#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


// max length input string
#define MAXSTR 120


int main( int argc, char *argv[]) {
      FILE *fp;
      if(argv[1] != NULL)
          fp = fopen(argv[1], "r");
       while(1 ) {

          // user input string
          char s[MAXSTR+3];

          // eof & ctr-d var
          char * x;

          if(fp == NULL){
                // prompt for input
                printf("msh> ");

                // get input and return for ctr-d check
                x = fgets(s, MAXSTR+3, stdin);

                // check input length; n does not include final carriage return
                int n = strlen(s)-1;

                // chech for valid input length
                if (n > MAXSTR) {
                      printf("error: input too long\n");
                }
          }
          else{
                x = fgets(s, MAXSTR+3, fp);
          }

          // tokenize string
          char * args[MAXSTR/2 + 1];
          s[strlen(s) - 1] = '\0';
          args[0] = strtok(s, " ");
          int i = 0;
          while(args[i] != NULL){
              args[++i] = strtok(NULL, " ");
          }

          // checks for exit
          if (x == NULL || (strcmp(args[0], "exit") == 0 && args[1] == NULL)) {
                exit(0);
          }

          // check for 'help'
          if (strcmp(args[0], "help") == 0 && args[1] == NULL){
                 printf("enter Linux commands, or ‘exit’ to exit\n");
          }

          // check for 'today'
          else if (strcmp(args[0], "today") == 0 && args[1] == NULL){
                 time_t now;
                 time(&now);
                 int month, day, year;
                 struct tm *today = localtime(&now);
                 month = today->tm_mon +  1;
                 day = today->tm_mday;
                 year = today->tm_year + 1900;
                 printf("%d/%d/%d\n", month, day, year);
          }

          else if(strcmp(args[0], "cd") == 0){

                  if(args[1] == NULL){
                      char *home = getenv("HOME");
                      chdir(home);
                  }
                  else{
                      // int errno =
                      if( chdir(args[1]) ){
                        printf("msh: cd: No such file or directory\n");
                      }

                  }
          }

          else {
                  int pid = fork();

                  // failed fork
                  if(pid < 0){
                      fprintf(stderr, "fork failed\n");
                      exit(1);
                  }

                  // child process
                  else if(pid == 0){
                        int errno = execvp(args[0] , args);
                        printf("eror %d\n", errno);
                        printf("msh: %s: %s\n", args[0], strerror(errno));
                        exit(1);


                    }

                    // parent process
                    else {
                       int rc_wait = wait(NULL);
                    }
            }

          }

    exit(0);
    }
