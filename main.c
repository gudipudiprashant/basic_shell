#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>


#define BUFFER_SIZE 100

void get_cur_dir(char *);

int main(){

	char *input,*cwd, *leave = "exit";
	
	input = (char *)malloc(sizeof(char)*BUFFER_SIZE);
	cwd = (char *)malloc(sizeof(char)*BUFFER_SIZE);



	while(1){

		//get current directory and print the prompt
		get_cur_dir(cwd);
		printf("basic_shell:%s$ ",cwd);


		//get input
		fgets(input,BUFFER_SIZE-1,stdin);
		input = strtok(input,"\n");

		if((*input) == (*leave)){
			break;
		}

		printf("%s\n",input);

	}

	return 0;
}

void get_cur_dir(char *cur_dir){

	cur_dir = getcwd(cur_dir,BUFFER_SIZE-1);

}