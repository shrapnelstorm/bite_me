// cpp includes
#include <iostream>
#include <string>

// c includes
#include <string.h>
#include <stdio.h>
using namespace std;


int main() {
	char user_input[80] ;
	const char delim[] = " \t\n" ;// TODO: add other delimiters

	scanf("%s", &user_input) ; // read first line and begin looping
	while (user_input != "exit") {

		// tokenize input
		char *fst_token, *snd_token ;
		fst_token = strtok(user_input, delim); 
		snd_token = strtok(NULL, delim) ;

		// begin parsing the inputs
		bool wait_for_children = true ;
		while ( fst_token != NULL) {
			printf("%s ## %s \n", fst_token, snd_token );

			// read next tokens
			// XXX: Does the first token change at all?
			fst_token = strtok(NULL, delim); 
			snd_token = strtok(NULL, delim) ;
		}

		// wait for process completion

		// read next input
		scanf("%s", &user_input) ; 
	}
}

int setup_pip(int &cur_output, int &next_input){
  int pip[2];
  int result;
  result = pipe(pip);
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
  execve(cmd,NULL,NULL);
}
