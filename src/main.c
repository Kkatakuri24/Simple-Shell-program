/**
 * @file: main.c
 * @author: Joshua Kyi, 578386773, CSCI 360, VIU
 * @version: 1.0.0
 * @modified: September 25, 2020
 *
 * Simple shell application.
 *
 */

/*
 * Include all library and custom header files.
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/360shell.h"
//get_command() switch later



int main(void) {
	
    char command[COMMAND_MAX_LENGTH]; // user enterd command

    char* args[COMMAND_MAX_LENGTH/2 + 1]; 	//argument list
    int args_length = -1;			//count of actual arguments in the list

   
    char histories[COMMAND_HISTORY_MAX_LENGTH][COMMAND_MAX_LENGTH];  //keeps track of command history 	
    int head = 0;     	// points to the oldest command in the command history
    int tail = 0;	// points to the recent command in the command history
    
    int redirect_position=-1;      // position of '>' or '<' or '|' character in the argument list    
    int concurrent = 0;  	// flag for concurrent processing

    char *redirect = "|";
    int length;

    for(int i=0; i<COMMAND_HISTORY_MAX_LENGTH; i++){
      *histories[i]='\0';
    }
    //run a loop until user enters "exit"
    while(check_exit(command)!=1){
      length=get_command(command);
      check_exit(command);
      if(check_history(command)==1){
	show_history(histories, head, tail);
      }else if(check_recent(command, length)==1){
	get_recent_command(histories, tail, command);
      }else{
	add_to_history(histories, &head, &tail, command);
	args_length = parse_command(command, args);
	redirect_position = check_redirect((const char**) args, args_length, redirect);
	if(redirect_position== -1){
	  handle_command(args, args_length, concurrent);
	}else if(redirect_position == 1){
	  handle_pipe(args, args_length, redirect_position);
	}else if(redirect_position == 2){
	  handle_redirect_output(args, redirect_position);
	}else if(redirect_position == 3){
	  handle_redirect_input(args, redirect_position);
	}
      }
    }
    
    //Inside the loop do the followings:
    //	Get user command through the keyboard and continue to get user command until user type a non-zero length command
    //	Once a non-zero length command is entered
    //		Check, whether the command is 'exit' or not, if it is 'exit' command, exit from the program.
    //		Check, whether the command is 'history' or not, if it is 'history' command, display the history
    //		and continue to get another user command.
    //		Check, whether the command is '!!', i.e., recent command, if it is, get the recent command from
    //		the history and proceed with it.
    //		Add the user command to the history, if the user command is none of the above.
    //		Parse the command and its arguments.
    //		Check whether the command or the list of arguments contains output redirection, if yes, handle
    //		the command with output redirection and continue to get another user command.
    //		Check whether the command contains input redirection, if yes, handle the command with input redirection
    //		and continue to get another user command.
    //		Check whether the command contains pipe redirection among two commands, if yes, handle the commands with
    //		pipe redirection and continue to get another user command.
    //		Check whether the command contains concurrent processing request, '&' at the end, set the concurency flag,
    //		Handle the command with concurrency flag and continue to get another user command.

    
    return 0;
}

