/**
 * @file: 360shell.c
 * @author: Joshua Kyi, 578386773, CSCI 360, Fall 2020, VIU
 * @version: 1.0.0
 * @modified: September 25, 2020
 *
 * Implements the functions required for simple shell application.
 *
 */

/*
 * Include library and custom header files.
 *
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"../include/360shell.h"
  
/*
 * Implement all the functions of 360shell.h header file.
 *
 */
//function to read the input command and return length
int get_command(char* command){
  printf("CSCI360> ");
  size_t length = 80;
  getline(&command, &length, stdin);
  command[strlen(command)-1] = 0; //get rid of the trailing new line char
  return length;
}

//function to check if the command is exit
int check_exit(const char* command){
  char* str = "exit";
  if(strcmp(command, str)==0){
    return 1;
  }else{
    return 0;
  }
}


//function to check if the command is history
int check_history(const char* command){
  char *str= "history";
  if(strcmp(command, str)==0){
    return 1;
  }else{
    return 0;
  }
}


//fucntion to check if the command is recent '!!', 0 otherwise
int check_recent(const char* command, const int len){
  char *str="!!";
  if(strcmp(command, str)==0){
    return 1;
  }else{
    return 0;
  }
}

//extract the space separated command and its arguments into the list of arguments 'args'
int parse_command(char* command, char *args[]){
  int i;
  for(i=0; i < COMMAND_MAX_LENGTH; i++){
    args[i] = strsep(&command, " ");

    if(args[i] == NULL)
      break;
    if(strlen(args[i])==0)
      i--;
  }
  return i;
}

//check whether concurrent execution of the processes has been requested by giving and '&' at the end of the command or not. return 1 of request, 0 otherwise.
int check_concurrent(const char** args, const int len)
{
  //char* ret = strchr(*args, concurrent);
  return 0;
  
}

//execute command
void handle_command(char** args, int len, const int concurrent){
  pid_t pid= fork();

  if(pid == -1){
    printf("Failed to fork Child...\n");
    return;
  }else if(pid == 0){
    if(execvp(args[0], args)<0){
      printf("Command not found.\n");
    }
    exit(0);
  }else{
    printf("WAITING for the child process to complete..........\n\n");
    wait(NULL);
    printf("...........Child completed.\n");
    return;
  }
}

//add the command into the history list
void add_to_history(char histories[][COMMAND_MAX_LENGTH], int* head, int* tail, const char* command){
  int i=0;
  for(i=0; i< COMMAND_HISTORY_MAX_LENGTH; i++){
    if(*histories[i]=='\0'){
      strcpy(histories[i], command);
      if(i==0){
	head=0;
      }else{
	*tail=i;
      }
      break;
    }
  }
  int j = 0;
  if(i>=COMMAND_HISTORY_MAX_LENGTH){
    for(j=0; j< COMMAND_HISTORY_MAX_LENGTH; j++){
      strcpy(histories[j], histories[j+1]);
    }
    strcpy(histories[i-1], command);
  }
}

//show history
void show_history(const char histories[][COMMAND_MAX_LENGTH], int head, int tail){
  for(int i=0; i<COMMAND_HISTORY_MAX_LENGTH; i++){
    if(*histories[i]!= '\0'){
      printf("%d %s\n", i, histories[i]);
    }
  }
}

//copy the most recent command from the command histories into 'command' parament
void get_recent_command(const char histories[][COMMAND_MAX_LENGTH], const int tail, char* command){
  if(*histories[tail] == '\0'){
    printf("no recent command yet\n");
  }else{
    strcpy(command, histories[tail]);
    printf("csci360> %s\n", command);    
  }
  
}

//Handle the input redirect
void handle_redirect_input(char** args, const int redirect_position){
  pid_t childOne;
  char *fileName;
  char *afterRedirect[COMMAND_MAX_LENGTH];
  for(int i=0; i< COMMAND_HISTORY_MAX_LENGTH; i++){
    afterRedirect[i]='\0';
  }

  int count = 0;
  while(args[count]!=NULL){
    count++;
  }
  //process the string
  char temp[COMMAND_MAX_LENGTH];
  char *temp2;
  for(int i=0; i< count; i++){
    strcat(temp, args[i]);
    if(i < count -1){
      strcat(temp, " ");
    }
  }
  temp2 = temp;
  char *token= strtok(temp2, "<");
  fileName= strtok(NULL, "< ");
  //grab the rest of it as command
  char *tempString = token;

  
  char *token2 = strtok(tempString, " ");
  int i=0;
  while(token2!=NULL){
    afterRedirect[i++]= token2;
    token2 = strtok(NULL, " ");
  }
  
  //create child
  childOne = fork();
  if(childOne < 0){
    printf("Bad child\n");
    exit(1);
  }
  if(childOne == 0 ){
    //child doing work
    printf("Waiting for child process to complete.......\n\n");
    int file_desc;
    if((file_desc = open(fileName, O_RDONLY))== -1){
      printf("Error opening file\n");
      exit(1);
    }
    dup2(file_desc, STDIN_FILENO);
    close(file_desc);
    execvp(afterRedirect[0], afterRedirect);
    printf("Failed to execute command\n");
    exit(1);
  }
  wait(NULL);
  wait(NULL);
  printf("..........Child process completed\n");
}


//Handle the output redirect
void handle_redirect_output(char** args, const int redirect_position){
  pid_t childOne; //initialize child process
  char *fileName;
  char *beforeRedirect[COMMAND_HISTORY_MAX_LENGTH];
  for(int i=0; i< COMMAND_HISTORY_MAX_LENGTH; i++){
    beforeRedirect[i]='\0';
    //printf("%s\n", args[i]);
  }

  int count = 0;
  while(args[count]!=NULL){
    // printf("%s\n", args[count]);
    count++;
  }
  //process the string
  char temp[COMMAND_MAX_LENGTH];
  char *temp2;
  for(int i=0; i< count; i++){
    strcat(temp, args[i]);
    if(i < count -1){
      strcat(temp, " ");
    }
  }
  
  temp2 = temp;
  //printf("%s\n",temp2);
  char *token= strtok(temp2, "> ");
  int i=0;
  beforeRedirect[i] =token;
  fileName = strtok(NULL, "> ");
  //printf("%s\n", fileName);
  //create child
  childOne= fork();
  if(childOne < 0){
    printf("Bad child\n");
    exit(1);
  }
  if(childOne == 0){
    //childOne as started doing work
    printf("Waiting for Child process to complete..........\n\n");
    int file_desc;
    file_desc = open(fileName, O_WRONLY | O_CREAT);
    dup2(file_desc, STDOUT_FILENO);
    close(file_desc);
    execvp(beforeRedirect[i], beforeRedirect);
  }
  //printf("done");
  wait(NULL);
  wait(NULL);
  printf("............Child process completed\n");
}

//Handle pipe
void handle_pipe(char** args, int len, const int pipe_position){
  int pipes[2]; //one for reading and other for writing
  pid_t childOne, childTwo;

  //piped argument list
  char *piped[len];
  char *beforePiped[len];
  char *temp2;
  for(int i =0; i< len; i++){
    piped[i]='\0';
    beforePiped[i]='\0';
    //printf("%s ", args[i]);
  }
  
  //cat the args back to string
  char temp[COMMAND_MAX_LENGTH];
  for(int i = 0; i < len; i++){
    strcat(temp, args[i]);
    if(i < len -1)
      {
	strcat(temp, " ");
      }
  }
  
  temp2 = temp;
  char *token = strtok(temp2, "| ");
  char *tempString = token;
  
  //grab the rest of the string separated
  char *z = strtok(NULL, "| ");
  int i = 0;
  while(z!=NULL){
    piped[i++] = z;
    z= strtok(NULL, "| ");
  }

  char *token2 = strtok(tempString, " ");
  int j =0;
  while(token2!=NULL){
    beforePiped[j++] = token2;
    token2 = strtok(NULL, " ");
  }
  
  //create pipe
  if(pipe(pipes)<0){
    printf("Error piping\n");
    return;
  }
  //create childOne
  childOne = fork();
  if(childOne < 0){
    printf("forking failed for child one\n");
  }
  
  if(childOne==0){
    //child one started
    close(pipes[0]);
    dup2(pipes[1], STDOUT_FILENO);
    close(pipes[1]);

    if(execvp(beforePiped[0], beforePiped) <0){
      printf("Command error\n");
      exit(0);
    }
  }else{
    childTwo= fork();
    if(childTwo<0){
      printf("forking failed\n");
      return;
    }
    //childtwo started
    if(childTwo == 0){
      close(pipes[1]);
      dup2(pipes[0], STDIN_FILENO);
      if(execvp(piped[0], piped) < 0){
	printf("command error\n");
      }
      wait(NULL);
      wait(NULL);
    }
  }
  close(pipes[0]);
  close(pipes[1]);
  //need to wait twice
  wait(NULL);
  wait(NULL);
}


//check whether the 'redirect is present in the argument list
int check_redirect(const char** args, const int len, const char* redirect){
  char *output= ">";
  char *input = "<";
  char *pipe = "|";
  
  for(int i=0; args[i]!=NULL; i++){
    if(strcmp(args[i], output)==0){
      return 2;
    }else if(strcmp(args[i], input)==0){
      return 3;
    }else if(strcmp(args[i], pipe)==0){
      return 1;
    }
  }
  return -1;
}
