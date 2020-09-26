/**
 * @file: 360shell.h
 * @author: Humayun Kabir, Instructor, CSCI 360, VIU
 * @version: 1.0.0
 * @modified: August 05, 2020
 *
 * Defines the functions required for simple shell application.
 *
 */


#ifndef __360_SHELL_HEADER__
#define __360_SHELL_HEADER__


#define COMMAND_MAX_LENGTH 80 			/* Maximum 80 chars per command */
#define COMMAND_HISTORY_MAX_LENGTH 10		/* Maximum 10 command histories */
#define RECENT_COMMAND_LENGTH 2			/* Length of '!!' command */
#define READ_END    0				/* Read end of pipe file descriptor pair */
#define WRITE_END   1				/* Write end of pipe file decriptor pair */



int get_command(char* command);
//Display 'CSCI360>' prompt to the user.
//Accept a command from the user through standard input (keyboard) and put it in 'command' parameter
//Trim the new line character from the end of user entered command
//Return the length of the command after trimming the new line character


int check_exit(const char* command);
//Return 1, if the command is 'exit' command, 0 otherwise.


int check_history(const char* command);
//Return 1, if the command is 'history' command, 0 otherwise.


int check_recent(const char* command, const int len);
//Return 1, if the command is recent '!!', 0 otherwise.


int parse_command(char* command, char *args[]);
//Extract the space separated command and its arguments into the list of arguments 'args'
//Return the number of arguments, including the command.


int check_concurrent(const char** args, const int len);
//Check whether concurrent execution of the processes has been requested by giving and '&' 
//at the end of the command or not.
//Return 1, if requested, 0 otherwise.


int check_redirect(const char** args, const int len, const char* redirect);
//Check whether the 'redirect' character is present in the argument list 'args' or not
//Return the position of the redirect character in the list if present, return -1 otherwise.


void add_to_history(char histories[][COMMAND_MAX_LENGTH], int* head, int* tail, const char* command);
//Add the 'command' at the appropriate position in the 'histories'.
//Update the 'head' and 'tail' pointers appropriately.


void show_history(const char histories[][COMMAND_MAX_LENGTH], const int head, const int tail);
//Loop through all the recent histories in order and display them in separate lines with the order number.
//Display the order numbers in the first column and the histories in the second column.


void get_recent_command(const char histories[][COMMAND_MAX_LENGTH], const int tail, char* command);
//Copy the most recent command from the command histories into 'command' parameter.



void handle_command(char** args, int len, const int concurrent);
//Create a child process to execute the command
//Inside the child process, end the argument list 'args' appropriately with a NULL argument.
//and replace the child process image with the image specified by 'args[0]'.
//Inside the parent process, wait for the child process to complete if concurrent processing is not
//requested. Inform the user appropriately.


void handle_redirect_output(char** args, const int redirect_position);
//Create a child process to execute the command.
//Inside the child process, end the argument list 'args' appropriately with a NULL argument.
//Open a file descriptor using the output file name in write only mode.
//Redirect the output by copying the opened file descriptor to the standard output file descriptor.
//Close the original file descriptor, since you don't need it anymore.
//Replace the child process image with the image specified by 'args[0]'.
//In the parent process, wait for the child process to complete.



void handle_redirect_input(char** args, const int redirect_position);
//Create a child process to execute the command.
//Inside the child process, end the argument list 'args' appropriately with a NULL argument.
//Open a file descriptor using the input file name in read only mode.
//Redirect the input by copying the opened file descriptor to the standard input file descriptor.
//Close the original file descriptor, since you don't need it anymore.
//Replace the child process image with the image specified by 'args[0]'.
//In the parent process, wait for the child process to complete.



void handle_pipe(char** args, int len, const int pipe_position);
//Create the first child process to execute the command before the pipe.
//Inside the first child process, open a pipe with two file descriptors, one for reading and 
//the other for writing, and create the second child process. 
//Inside the second child process, close the write file descriptor of the pipe,
//redirect its input by copying the read file descriptor of the pipe to the standard input file
//descriptor, end the argument list 'args' of the command after pipe appropriately with a NULL argument,
//replace the child process image with the image specified by the command after the pipe.
//Inside the first child process, close the read file descriptor of the pipe, redirect its
//output by copying the write descriptor of the pipe to the standard output file descriptor,
//end the argument list 'args' of the command before the pipe appropriately with a NULL argument,
//replace the child process image with the image specified by the command before the pipe.
//In the parent process, wait for all the child processes to complete.



#endif
