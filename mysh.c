#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
typedef struct job {
  int jid;
  int pid;
  int terminated;
  int background;
  int numOfArgs;
  char** arguments;
  } job;

int execute(int background, char *args[], int numOfArgs);
job* back_jobs;
int k = 0;
int count = 0;
int start = 0;
job newJob;
int main(int argc, char *argv[]) {
  if (argc > 2) {
    write(2, "Usage: mysh [batchFile]\n", 24);
    exit(1);
  }
    char myshell[]="mysh> ";
    char line[514];
    back_jobs = malloc(32*sizeof(job));
    newJob.arguments = malloc(20*sizeof(char*));
    int status;
    char* token;
    char** args;
    args = malloc (20*sizeof(char*));
  if (argc == 1) {
    do {
      write(1, myshell, 6);
      if (fgets(line, 514, stdin) == NULL) {
        write(1, "\n", strlen("\n"));
        exit(0);
      }
      if (strchr(line, '\n') == NULL) {
        line[513] = '\0';
        write(1, line, strlen(line));
        continue;
      }
      token = strtok(line, " \t");
      int j = strlen(token)-1;
      if (token[j] == '\n')
        token[j] = '\0';
      int i = 0;
      while ( token != NULL ) {
       j = strlen(token);
      if (token[j-1] == '\n') {
       token[j-1] = '\0';
       }
       args[i] = malloc(sizeof(char*));
       args[i] = strdup(token);
       newJob.arguments[i] = malloc(sizeof(char*));
       newJob.arguments[i] = strdup(token);
       token = strtok(NULL, " \t");
       i++;
    }
    int d;
    for (d = i; d > 0 ; d--) {
      if (strcmp(newJob.arguments[d-1], "") != 0) {
         i = d;
         break;
      }
    }
    if (strcmp(newJob.arguments[0], "") != 0) {
    if (strcmp(newJob.arguments[i-1], "&") == 0) { 
      newJob.arguments[i-1] = NULL;
      args[i-1] = NULL;
      newJob.numOfArgs = i-1;
      newJob.background = 1;
    } else {
      newJob.arguments[i] = NULL;
      args[i] = NULL;
      newJob.numOfArgs = i;
      newJob.background = 0;
    }
    int length = strlen(args[newJob.numOfArgs-1])-1;
    if (args[newJob.numOfArgs-1][length] == '&') {
      args[newJob.numOfArgs-1][length] = '\0';
      newJob.background = 1;
      }
    status = execute(newJob.background, args, newJob.numOfArgs);
    for (i = 0; i < newJob.numOfArgs; i++) {
      free(newJob.arguments[i]);
      }
    }
  } while (status);
}
  if (argc == 2) {
    FILE * fp;
    char* batchFile = strdup(argv[1]);
    fp = fopen(batchFile, "r");
    if (fp == NULL) {
      char* temp = "Error: Cannot open file ";
      write(2, temp, strlen(temp));
      char* temp2 = strdup(argv[1]);
      write(2, temp2, strlen(temp2));
      write(2, "\n", strlen("\n"));
      free(temp2);
      exit(1);
    }
      fgets(line, 514, fp);
      if (strcmp(line, "") == 0)
        exit(0);
      do {
      if (strchr(line, '\n') == NULL) {
        line[513] = '\0';
        write(1, line, strlen(line));
        continue;
      }
      write(1, line, strlen(line));
      token = strtok(line, " \t");
      int j = strlen(token)-1;
      if (token[j] == '\n')
        token[j] = '\0';
      int i = 0;
      while ( token != NULL ) {
       j = strlen(token);
      if (token[j-1] == '\n') {
       token[j-1] = '\0';
       }
       args[i] = malloc(j*sizeof(char)+1);
       args[i] = strdup(token);
       newJob.arguments[i] = malloc(j*sizeof(char)+1);
       newJob.arguments[i] = strdup(token);
       token = strtok(NULL, " \t");
       i++;
    }
    int d;
    for (d = i; d > 0 ; d--) {
      if (strcmp(newJob.arguments[d-1], "") != 0) {
         i = d;
         break;
      }
    }
    if (strcmp(newJob.arguments[0], "") != 0) {
    if (strcmp(newJob.arguments[i-1], "&") == 0) {
      newJob.arguments[i-1] = NULL;
      args[i-1] = NULL;
      newJob.numOfArgs = i-1;
      newJob.background = 1;
    } else {
      newJob.arguments[i] = NULL;
      args[i] = NULL;
      newJob.numOfArgs = i;
      newJob.background = 0;
    }
    if (newJob.numOfArgs > 0) {
      int length = strlen(args[newJob.numOfArgs-1])-1;
      if (args[newJob.numOfArgs-1][length] == '&') {
        args[newJob.numOfArgs-1][length] = '\0';
        newJob.background = 1;
      }
      status = execute(newJob.background, args, newJob.numOfArgs);
    }
    for (i = 0; i < newJob.numOfArgs; i++) {
      free(newJob.arguments[i]);
      free(args[i]);
      }
    }
    } while (status && fgets(line, 514, fp));
    fclose(fp);
    free(batchFile);
  }
  int i, j;
  for (i = 0; i < 32; i++) {
    for (j = 0; j < back_jobs[i].numOfArgs; j++) {
      free(back_jobs[i].arguments[j]);
      free(newJob.arguments[i]);
      free(args[i]);
      }
    free(back_jobs[i].arguments);
  }
  free(back_jobs);
  free(args);
  free(newJob.arguments);
  return EXIT_SUCCESS;
}

int createProcess(int background, char *args[]) {
  int pid, wpid;
  int status;
  int fd;
  pid = fork();
  count++;
  newJob.jid = count;
  if (pid == 0) {
    newJob.terminated = 0;
    if (background == 0) {
      execvp(args[0], args);
    } else if (background == 1) {
      close(STDIN_FILENO);
      (void) open("/dev/null", O_CREAT | O_RDONLY | O_TRUNC, S_IRWXU);
      // close(STDOUT_FILENO);
      // fd = open("/dev/null", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
      execvp(args[0], args);
    } else {
      count--;
      }
    char* str = ": Command not found\n";
    char* temp = strdup(args[0]);
    write(2, temp, strlen(temp));
    write(2, str, strlen(str));
    free(temp);
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    write(1, "error", 5);
  } else {
    // Parent process
    do {
      if (background == 1) {
         wpid = waitpid(pid, &status, WNOHANG|WUNTRACED);
         newJob.pid = wpid;
         if (wpid == pid) {
           newJob.terminated = 1;
         } else {
           newJob.terminated = 0;
           }
         if (k < 32) {
         back_jobs[k].terminated = newJob.terminated;
         back_jobs[k].background = newJob.background;
         back_jobs[k].numOfArgs = newJob.numOfArgs;
         back_jobs[k].jid = newJob.jid;
         back_jobs[k].pid = newJob.pid;
         int i, j;
         back_jobs[k].arguments = malloc(sizeof(newJob.arguments));
         back_jobs[k].arguments = newJob.arguments;
         for (i = 0; i < back_jobs[k].numOfArgs; i++) {
           back_jobs[k].arguments[i] = malloc(sizeof(char*));
           back_jobs[k].arguments[i] = newJob.arguments[i];
         }
         k++;
         } else {
            int i, d, j;
            for (d = start; d < 32; d++) {
               if (waitpid(back_jobs[d].pid, &status, WNOHANG) > 0) {
                 for (j = 0; j < back_jobs[d].numOfArgs; j++) {
                  free(back_jobs[d].arguments[j]);
                 }
                 free(back_jobs[d].arguments);
                 break;
               }
            }
            back_jobs[d].terminated = newJob.terminated;
            back_jobs[d].background = newJob.background;
            back_jobs[d].numOfArgs = newJob.numOfArgs;
            back_jobs[d].jid = newJob.jid;
            back_jobs[d].pid = newJob.pid;
            back_jobs[k].arguments = malloc(sizeof(newJob.arguments));
            back_jobs[k].arguments = newJob.arguments;
            for (i = 0; i < back_jobs[k].numOfArgs; i++) {
              back_jobs[k].arguments[i] = malloc(sizeof(char*));
              back_jobs[k].arguments[i] = newJob.arguments[i];
            }
            start++;
            if (start == 32)
              start = 0;
            }
       } else {
         wpid = waitpid(pid, &status, WUNTRACED);
         newJob.pid = wpid;
         }
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

/*
  Function Declarations for builtin shell commands:
 */
int myw(char *args[]);
int my_j();
int my_exit();
/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "myw",
  "j",
  "exit"
};

int myw(char *args[]) {
  int t = 0;
  time_t t1, t2;
  char* str = malloc(5*sizeof(char));
  char* temp = malloc(100*sizeof(char));
  char* temp1 = malloc(100*sizeof(char));
  char* str1 = malloc(10*sizeof(char));
  strcpy(temp, "0 : Job ");
  int i, status;
    for (i = 0; i < 32; i++) {
      if (back_jobs[i].jid == atoi(args[1])) {
        back_jobs[i].terminated = 1;
        sprintf(str, "%d", back_jobs[i].jid);
        strcat(temp, str);
        strcat(temp, " terminated\n");
        if (waitpid(back_jobs[i].pid, &status, WNOHANG) == 0) {
          time(&t1);
          waitpid(back_jobs[i].pid, &status, WUNTRACED);
          time(&t2);
          sprintf(str1, "%ld", (t2-t1)*1000000);
          strcat(temp1, str1);
          strcat(temp1, " : Job ");
          strcat(temp1, str);
          strcat(temp1, " terminated\n");
          write(1, temp1, strlen(temp1));
          } else {
            write(1, temp, strlen(temp));
          }
        break;
      } else {
         t++;
         if (t > k) {
           char str2[13] = "Invalid jid ";
           char* temp2 = strdup(args[1]);
           write(2, str2, strlen(str2));
           write(2, temp2, strlen(temp2));
           write(2, "\n", strlen("\n"));
           free(temp2);
           break;
         }
      }
    }
  free(str);
  free(temp);
  free(str1);
  free(temp1);
  return 1;
}

int my_j() {
  int i, j, d;
  int status;
  char* str = malloc(5*sizeof(char));
  for (i = 0; i < 32; i++) {
    if (waitpid(back_jobs[i].pid, &status, WNOHANG) == 0
      && back_jobs[i].jid > 0) {
      sprintf(str, "%d", back_jobs[i].jid);
      write(1, str, strlen(str));
      write(1, " : ", strlen(" : "));
      for (j = 0; j < back_jobs[i].numOfArgs-1; j++) {
        int length = strlen(back_jobs[i].arguments[j]);
        for (d = 0; d < length ; d++) {
          sprintf(str, "%c", back_jobs[i].arguments[j][d]);
          write(1, str, strlen(str));
        }
        write(1, " ", strlen(" "));
      }
      int length = strlen(back_jobs[i].arguments[j]);
      if (back_jobs[i].arguments[j][length-1] == '&')
        length--;
      for (d = 0; d < length ; d++) {
        sprintf(str, "%c", back_jobs[i].arguments[j][d]);
        write(1, str, strlen(str));
      }
      write(1, "\n", strlen("\n"));
    }
  }
  free(str);
  return 1;
}
int my_exit() {
  return 0;
}

int execute(int background, char *args[], int numOfArgs) {
  int i;

  if (args[0] == NULL) {
    return 1;
  }
  char* str = args[0];
  args[0] = strtok(str, "\t\r\n\v\f");
  if (strcmp(args[0], "j") == 0) {
    if (numOfArgs == 1)
      return my_j();
    if (numOfArgs == 2 && strcmp(args[1], "&") == 0)
      return my_j();
  }
  if (strcmp(args[0], "myw") == 0) {
    if (numOfArgs < 2) {
      char* temp = strdup(args[0]);
      char* str = ": Command not found\n";
      write(2, temp, strlen(temp)+1);
      write(2, str, strlen(str)+1);
      free(temp);
    }
    if (numOfArgs == 2)
      return myw(args);
    if (numOfArgs == 3 && strcmp(args[2], "&") == 0)
      return myw(args);
  }
  if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "exit&") == 0) {
    if (numOfArgs == 1)
      return my_exit();
    if (numOfArgs == 2 && strcmp(args[1], "&") == 0)
      return my_exit();
  }
  return createProcess(background, args);
}




