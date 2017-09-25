#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "mytoc.h"
#include "utility.h"

void printVec(char **tokenVec){
  print("~~~~Vector~~~~\n");
  for(;*tokenVec; tokenVec++){
    print(*tokenVec);
  }
  print("~~~~~~~~~~~~~~\n");
}

void freeTokens(char **tokenVec){
  for(;*tokenVec; tokenVec++)
    free(*tokenVec);
}

char **getPATH(char **envp){
  char **temp;
  // get the PATH string
  for(int i=0; envp[i]; i++){
    temp = mytoc(envp[i], '=');

    if(!strcmp(temp[0], "PATH"))
          break;
    freeTokens(temp);
    free(temp);
  }
  char **PATH = mytoc(temp[1], ':');
  
  freeTokens(temp);
  free(temp);
  return PATH;
}

char *findPath(char **argv, char **envp){
  char *temp = '\0';
  // get environment variable
  char **PATH = getPATH(envp);
  // search for command
  char *local = strcat("./", *argv);
  if(!access(local, X_OK)){
    // executable is in local directory
    temp = local;
  }
  else if(*argv[0] == '/' && !access(*argv, X_OK)){
    // path was already provided
    temp = *argv;
  }
  else{
    // search all directories in PATH
    char **current = PATH;
    while(*current){
      *current = strcat(*current, "/");
      *current = strcat(*current, *argv);
      if(!access(*current, X_OK)){
	// found command
	temp = *current;
	break;
      }
      current++;
    }
  }

  char *path = calloc(strlen(temp)+1, sizeof(char));
  for(int i=0; temp[i]; i++)
    path[i] = temp[i];
  
  free(local);
  freeTokens(PATH);
  free(PATH);
  return path;
}

void runCommand(char **argv, char **envp){
  // get path for the executable
  char *path = findPath(argv, envp);;
  if(*path){
    int retVal = execve(path, argv, envp);
    print("Program terminated with exit code \n");
    // print exit code
    free(path);
  }
  else {
    print("command not found\n");
  }
  
  free(path);
  exit(0);
}

int main(int argc, char **argv, char **envp){
  char buf[128];

  for(;;){    
    write(1, "$ ", 2);
    int bufSize = read(0, buf, 128);
    buf[bufSize-1] = '\0';
    // tokenVec will be the argv for the command
    char **tokenVec = mytoc(buf, ' ');
    
    // Check if exiting the program
    if(tokenVec[0] &&
       !strcmp(tokenVec[0], "exit") &&
       !tokenVec[1])
	break;

    pid_t pid = fork();
    if(pid == 0)
      // run command with child process
      runCommand(tokenVec, envp);
    else
      // parent waits for the child to finish
      wait(NULL);

    freeTokens(tokenVec);
    free(tokenVec);
  }
  print("Laterz! \\(^.^)/\n");
}
