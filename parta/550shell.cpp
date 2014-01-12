/*#include <iostream>
#include <sstream>
#include <string>
*/

// c includes
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

// function prototypes
int setup_pip(int &cur_output, int &next_input);
int init_child(char* cmd, int input, int output);

const int STD_IN = 0 , STD_OUT = 1 ;
int main() {
	//char user_input[100] ;
	char *user_input = NULL ;
	size_t len = 0 ;
	ssize_t read ;

	const char delim[] = " \t\n" ;// TODO: add other delimiters


	getline(&user_input, &len, stdin) ; // read first line and begin looping

	while (strcmp(user_input, "exit") != 0) {
		printf("%s", ":)>\n"); 

		char *fst_token, *snd_token ;
		fst_token = strtok(user_input, delim); 
		/*
		while (fst_token != NULL)
		{
			printf("%s \n", fst_token) ;
			fst_token = strtok(NULL, delim) ;
		}
		exit(0) ;
		*/
		snd_token = strtok(NULL, delim) ;

		// begin parsing the inputs
		bool wait_for_children = true ;
		bool forked = false ;

		// input and output file descriptors
		int cur_input = STD_IN, next_input = STD_IN ;
		int cur_output = STD_OUT, next_output = STD_OUT ;
		pid_t last_pid ;
		while ( fst_token != NULL) {




		//printf("Tokens are %s ## %s\n", fst_token, snd_token) ;
			//printf("first arg: %s second arg: %s", fst_token, snd_token) ;
			if (snd_token != NULL && strcmp(snd_token, "|") == 0) {
				//setup_pip(cur_output, next_input);
				forked = true ;
  int pip[2];
  int result;
  result = pipe(pip);
  fflush(stdout) ;
  if(result == -1){
    perror("Unable to create a pipe");
    exit(1);
  }
  else {
    cur_output = pip[1];
    next_input = pip[0];
  }
  printf("created pipe: i -- %i out -- %i\n", pip[0], pip[1]) ;

			}
			else if (snd_token != NULL && strcmp(snd_token, "&") == 0) {
				wait_for_children = false ;
			}
			
			//printf("ni:%i\n no: %i \nci: %i co: %i \n", next_input, next_output,  cur_input, cur_output);
			//printf("%s", "i'm forking") ;
  			fflush(stdout) ;
			last_pid = fork() ;
			//printf("%s %i", "i forked with ", last_pid) ;
			if( 0 == last_pid ) {
				//printf("Inside child with cmd %s", fst_token) ;
				printf("fst token: %s\n ", fst_token) ;
				init_child(fst_token, cur_input, cur_output) ;
			} else {
				if (forked && cur_output > 1) {
					close(cur_output) ;
				}
				if (forked && cur_input > 1) {
				}
			}
			//printf("Inside parent with cmd %s", fst_token) ;

			// update I/O due to piping
			cur_input = next_input ;
			cur_output = STD_OUT ; 
			//printf("ni:%i\n no: %i \nci: %i co: %i \n", next_input, next_output,  cur_input, cur_output);
			
			// read next tokens
			// XXX: Does the first token change at all?
			fst_token = strtok(NULL, delim); 
			snd_token = strtok(NULL, delim) ;

			int status ;
			// TODO: do we want any options here??
			waitpid(last_pid, &status, 0 ) ;
		}

		
		/*
		// wait for process completion
		if (wait_for_children){
			int status ;
			// TODO: do we want any options here??
			waitpid(last_pid, &status, 0 ) ;
			//wait(0) ;
		}
		*/

		// read next input
		getline(&user_input, &len, stdin) ; // read first line and begin looping
	}
	free(user_input) ;
}

int setup_pip(int &cur_output, int &next_input){
  int pip[2];
  int result;
  result = pipe(pip);
  fflush(stdout) ;
  if(result == -1){
    perror("Unable to create a pipe");
    exit(1);
  }
  else {
    cur_output = pip[1];
    next_input = pip[0];
  }
  printf("created pipe: i -- %i out -- %i\n", pip[0], pip[1]) ;
  return 1;
}

int init_child(char* cmd, int input, int output){
printf("%s , i:%i\n o:%i \n", cmd, input, output) ;
  int result1,result2;
  result1 = dup2(input,0);
  if(result1 == -1){
    perror("dup2 failed");
    exit(1);
  }
  result2 = dup2(output,1);
  if(result2 == -1){
    perror("dup2 failed");
    exit(1);
  }

  //printf("%s", cmd) ;
  fflush(stdout) ;
  char* args[] = {cmd, NULL} ; 
  char* env[] = {NULL} ;
  //perror("running cmd ") ;
  execvp(cmd,args);
  
}
