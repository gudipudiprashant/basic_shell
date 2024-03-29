#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include"main.h"
#include"inbuilt.c"
#include"exec.c"

#define PRINT_CMD 0


int main(){

	pid_t pid;
	int status;

	int i,is_inp_empty;
	
	initialize();

	//need to be done only once in the program.
	ar_his = (char **)malloc(sizeof(char *)*(HIS_SIZE+1));
	cur_p = (queue_p *)malloc(sizeof(queue_p));



	while(1){


		//print the prompt
		printf("basic_shell:%s$ ",pwd);
		
		//get command entered into memory pointed by input
		is_inp_empty = get_input();

		if(is_inp_empty){
			continue;
		}

		access_history(input);


		//add input into history
		maintain_his(input);

		//parse input: Serarate it into commands and execute
		parse_inp(input);

		
		if(PRINT_CMD){
			print_command();
		}


		free(input);
		free(args); 
		//only freeing the array coz the other pointers 
		//point to locations in input block and that is already freed.
	}

	return 0;
}


int get_input(void){

	//takes input from the user.
	//returns 1 if the string is empty.
	//returns 0 if the string is not-empty.
	
	input = (char *)malloc(sizeof(char)*BUFFER_SIZE);
	
	fgets(input,BUFFER_SIZE-1,stdin);

	//fgets has \n in the end of the input string.

	//check for empty string
	if(strcmp(input,"\n") == 0){
		return 1;
	}

	// Trim tariling newline
    input[strcspn(input,  "\n")] = 0;

	 //input = strtok(input,"\n"); //removes the \n at the end of the string.

	return 0;
	
}



void print_command(void){

	int token_pos = 0;

	printf("\nCommand: ");

	while(args[token_pos]!=NULL){
		printf("%s ",args[token_pos]);
		token_pos++;
	}
	printf("\n");
}