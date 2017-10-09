#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include "mytoc.h"
#include "utility.h"

#define READ_END 0
#define WRITE_END 1

void printVec(char **tokenVec){
  println("~~~~Vector~~~~");
  for(;*tokenVec; tokenVec++){
    println(*tokenVec);
  }
  println("~~~~~~~~~~~~~~");
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

  // get each directory
  char **PATH = mytoc(temp[1], ':');
  
  freeTokens(temp);
  free(temp);
  return PATH;
}

char *findPath(char **argv, char **envp){
  char *temp = "";
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
  char *path = findPath(argv, envp);

  // execute if it exists
  if(*path){
    int retVal = execve(path, argv, envp);
    printerr("Program terminated with exit code \n");
    // TODO: print exit code
  }
  else {
    printerr("command not found\n");
  }
  
  free(path);
  exit(0);
}

void runCommands(char **command, char **envp){
  char **argv0 = mytoc(command[0], ' ');
  char **argv1 = mytoc(command[1], ' ');
  int stdin_copy = dup(0);
  int stdout_copy = dup(1);
  pid_t pid, pid2;
  int pipefd[2];

  pipe(pipefd);
  pid = fork();
  if(pid == 0){
    // run first command with child
    // redirect ouput to pipe
    close(1);
    dup(pipefd[1]);
    close(pipefd[0]);
    close(pipefd[1]);
    // execute the command
    runCommand(argv0, envp);
  }
  else{
    // redirect input to pipe
    close(0);
    dup(pipefd[0]);
    close(pipefd[1]);
    close(pipefd[0]);
    
    pid2 = fork();
    if(pid2 == 0){
      // run second command with child
      runCommand(argv1, envp);
    }
  }

  //wait for both children
  wait(NULL);
  wait(NULL);
  // restore stdout and stdin
  dup2(stdin_copy, 0);
  dup2(stdout_copy, 1);
}

int main(int argc, char **argv, char **envp){
  char buf[128];
  pid_t pid;

  for(;;){    
    //print("$ ");
    int bufSize = read(0, buf, 128);
    // remove newline char
    buf[bufSize-1] = '\0';
    char **tokenVec = mytoc(buf, ' ');

    // check if input was given
    if(!tokenVec[0])
      continue;
    // check if exiting the program
    if(!strcmp(tokenVec[0], "exit") && !tokenVec[1])
      break;

    //check if chaging directory
    if(!strcmp(tokenVec[0], "cd")){
      if(tokenVec[1]){
	int retVal = chdir(tokenVec[1]);
	if(retVal)
	  printerr("unable to change directory\n");
      }
      else{
	printerr("no directory given\n");
      }
      continue;
    }

    // perform commands
    int count = countTokens(buf, ' ');
    char **command = mytoc(buf, '|');
    if(command[0] && command[1]){
      // a pipe was entered
      runCommands(command, envp);
    }
    else if(count > 1 && !strcmp(tokenVec[count-1], "&")){
      // a background command was entered
      pid = fork();
      if(pid == 0){
	tokenVec[count-1] = '\0';
	free(tokenVec[count]);
	runCommand(tokenVec, envp);
      }
    }
    else {
      // a simple command was entered
      pid = fork();
      if(pid == 0){
	runCommand(tokenVec, envp);
      }
      else{
	wait(NULL);
      }
    }

    // clean up
    freeTokens(command);
    free(command);
    freeTokens(tokenVec);
    free(tokenVec);
  }
  print("Laterz! \\(^.^)/\n");
}
