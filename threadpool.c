#include "threadpool.h"
#include "pq.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *exectask(void *arg){
	task_t *task = (task_t*)malloc(sizeof(task_t));
	if(task == NULL){
		perror("Malloc");
		exit(0);
	}
	task->job = NULL;
	task->argument = NULL;
	//loop infinetly, getting the next highest priority from the queue and executing
	//the function pointed to by task->job
	while(1){
		task = (task_t*)(pq_next(arg));
		task->job(task->argument);
	}
}

threadpool_t *tp_create(int nthreads){
	threadpool_t *pool = (threadpool_t*)malloc(sizeof(threadpool_t));
	if(pool == NULL){
		perror("Malloc");
		exit(0);
	}
	pool->q = pq_create(); //create queue
	for(int i = 0; i < nthreads; i++){
		//create nthreads number of threads
		if(pthread_create(&(pool->tid[i]), NULL, &exectask, pool->q) != 0){
			perror("Thread error");
			exit(0);
		}
	}
	return pool;
}

void tp_submit(threadpool_t *tp, void (*job)(void *), void *arg, short prio){
	task_t *tasks = (task_t*)malloc(sizeof(task_t));
	if(tasks == NULL){
		perror("Malloc");
		exit(0);
	}
	//submit task for execution
	tasks->job = job;
	tasks->argument = arg;
	pq_insert(tp->q, tasks, prio);
} 
