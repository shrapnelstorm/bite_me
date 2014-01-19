#include<iostream>
#include<stdio.h>
#include <sys/mman.h>
#include <pthreads.h>

#define FILESIZE (1000 * sizeof(char)) // how to fix this?
#define STD_IN 0
#define STD_OUT 1
#define thread_no 50

pthread_mutex_t thread_avail[thread_no] = {PTHREAD_MUTEX_INITIALIZER}
int avail[k];

struct Thread_input{
	int socket_id;
	char *filename;
};

class Thread_output{
	void *memory_map;
	int socket_id;
	int fd;
public: Thread_output(void *mmp,int s_id, int fd);
};

struct thread_pool_input{
	int pipe_write;
	int socket_id;
	
};
Thread_output::Thread_output(void *mmp, int s_id){
	memory_map = mmp; // do we need a copy constructor here?
	socket_id = s_id;
	fd = fd;
}

class Thread_pool{
	private:
		TwoWayPipe p;
		TwoWayPipe pipe_threads[k];
	public:
		Thread_pool(TwoWayPipe p);
		void *helper_thread(void *pipe_rw);
		void *thread_pool(void *input);
		
}

class Thread_pipe{
	int read;
	int write;
	int thread_id;
	int pipe_write;
};

Thread_pool::Thread_pool(TwoWayPipe p){
	p = p;
}
Thread_pool::void *helper_thread(void *thread_id){
	int fd;
	char* map;
	int result;
	Thread_input input;
	//result = dup2(pipe_rw.read, STD_IN);
	//if(result == -1){
	//	perror("dup2 failed");	
	//}	
	while(1){
		int read_size = read(pipe_threads[thread_id].tp_read, input, sizeof(input));
		fd = open(input.filename, O_RDONLY);
		if(fd == -1){
			perror("Error opening file");
			// should we exit here?	
		}
		map = mmap(0,FILESIZE, PROT_READ, MAP_SHARED,fd,0);
		if(map = MAP_FAILED){
			// how should we respond? should we keep trying?	
		}
		
		Thread_output output(map,input.socket_id,fd);
		// send output via pipe
		write(p.tp_write,output,sizeof(output));
		pthread_mutex_lock(&thread_avail[thread_id]);
		avail[thread_id] = 1;
		pthread_mutex_unlock(&thread_avail[thread_id]);
	}
}

Thread_pool::void *thread_pool(void *input){
	pthread_t thread[k];
	int i;
	for(i=0;i<k;i++){
		pthread_create(&thread[i],NULL,helper_thread,(void *)i);	
	}
	Thread_input input;
	while(1){
		read(p.tp_read,input,sizeof(input));
		for(i=0;i<k;i++){
			pthread_mutex_lock(&thread_avail[i]);
			if(avail[i]){
				avail[i] = 0;
				pthread_mutex_unlock(&thread_avail[i]);
				break;			
			}
			pthread_mutex_unlock(&thread_avail[i]);		
		}
		write(pipe_threads[i].s_write,input,sizeof(input));
	}		
}
