Authors:		Sivaramakrishnan Natarajan Ramamoorthy	[1323328] (sivanr@cs.washington.edu)
				Armando Diaz Tolentino	[1323261] (ajdt@cs.washington.edu)

Course: 		CSE 550
Instructor:		Arvind Krishnamurthy
Quarter:		Winter 2014

Desc:			This repository contains code for the first assignment
				of the above course: implementing a basic bash shell
				and an AMTED server.

#### Shell Usage Instructions ########################

# run the shell
./550shell

# user prompt
the prompt :)> is printed when the shell is ready for input

# to exit
type exit at the prompt

#### features 
* & is implemented to run a command in the background

# known issues:
* commands and pipe symbol must be space separated
	* "ls | wc " is ok, but not "ls|wc"

#### AMTED SERVER Usage Instructions ########################
# run the server
./550server <address> <port>
NOTE: the server will not run if these arguments aren't provided



