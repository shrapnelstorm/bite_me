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
	// read user input
	char *user_input		= NULL ;
	size_t len 				= 0 ;
	ssize_t chars_read ;
	const char delim[] 		= " \t\n" ;
	printf("%s", ":)>\n");  /* prompt */
	chars_read = getline(&user_input, &len, stdin) ; 

	while ( strcmp(user_input, "exit") != 0) {

		char *fst_token, *snd_token ;
		fst_token = strtok(user_input, delim); 
		snd_token = strtok(NULL, delim) ;

		// begin parsing the inputs
		bool wait_for_children = true ;
		bool forked = false ;

		// input and output file descriptors
		int cur_input = STD_IN, next_input = STD_IN ;
		int cur_output = STD_OUT, next_output = STD_OUT ;
		pid_t last_pid ;
		while ( fst_token != NULL) {

			if (snd_token != NULL && strcmp(snd_token, "|") == 0) {
				setup_pip(cur_output, next_input);
				forked = true ;
			}
			else if (snd_token != NULL && strcmp(snd_token, "&") == 0) {
				wait_for_children = false ;
			}
			
  			fflush(stdout) ;
			last_pid = fork() ;
			if( 0 == last_pid ) {
				init_child(fst_token, cur_input, cur_output) ;
			} else {
				if (forked && cur_output > 1) {
					close(cur_output) ;
				}
				if (forked && cur_input > 1) {
				}
			}

			// update I/O due to piping
			cur_input = next_input ;
			cur_output = STD_OUT ; 
			
			// read next tokens
			// XXX: Does the first token change at all?
			fst_token = strtok(NULL, delim); 
			snd_token = strtok(NULL, delim) ;

			int status ;
			// TODO: do we want any options here??
			waitpid(last_pid, &status, 0 ) ;
		}
		
		// read next input
		printf("%s", ":)>\n");  /* prompt */
		getline(&user_input, &len, stdin) ; // read first line and begin looping
		printf("%s", user_input) ;
		printf("%i", strcmp(user_input, "exit")) ;
	}
	// free user input buffer used by getline
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
  return 1;
}

int init_child(char* cmd, int input, int output){
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

  fflush(stdout) ;
  char* args[] = {cmd, NULL} ; 
  char* env[] = {NULL} ;
  execvp(cmd,args);
  
}
