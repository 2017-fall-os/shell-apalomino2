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
  char **PATH = getPATH(envp);

  int exeVal;
  char *local = strcat("./", *argv);
  if(access(local, X_OK)){
    // executable isn't in the local directory
    char **current = PATH;
    // search all directories in PATH
    while(*current){
      *current = strcat(*current, "/");
      *current = strcat(*current, *argv);
      print(*current);
      print("\n");
      if(!access(*current, X_OK)){
	return *current;
	break;
      }
      current++;
    }
  }
  else{
    // executable is in local directory
    return local;
  }
}
	       
int runCommand(char **argv, char **envp){
  // get path for the executable
  char *path = findPath(argv, envp);

  if(!*path){
    print("command not found\n");
  }
  else {
    
  }
  int retVal = execve(path, argv, envp);
  print("error running command\n");
  
  free(path);
}

int main(int argc, char **argv, char **envp){
  char buf[128];

  for(;;){    
    write(1, "$ ", 2);
    int bufSize = read(0, buf, 128);
    buf[bufSize-1] = '\0';
    char **tokenVec = mytoc(buf, ' ');
    
    // Check if exiting the program
    if(tokenVec[0] &&
       !strcmp(tokenVec[0], "exit") &&
       !tokenVec[1])
	break;

    pid_t pid = fork();
    if(pid == 0)
      runCommand(tokenVec, envp);
    else
      wait(NULL);

    freeTokens(tokenVec);
    free(tokenVec);
  }
  print("Laterz! \\(^.^)/\n");
}
