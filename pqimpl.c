#include "pq.h"
#include <malloc.h>

pq_t *pq_create(void){
	pq_t *q = (pq_t*)malloc(sizeof(pq_t));
	q->numNodes = 0;
	pthread_mutex_init(&q->mx, NULL);
	pthread_cond_init(&q->cv, NULL);
	return q;
}

void pq_insert(pq_t *q, void *item, short prio){
	//create a new node, allocate memory and initialize
	node *newNode = (node*)malloc(sizeof(node));
	newNode->value = item;
	newNode->priority = prio;
	newNode->next = NULL;
	//if head of list is empty, make newNode the head
	if(!q->head){
		q->head = newNode;
	}
	//if priority of head is less than new prio, set new prio to head of list
	else if(q->head->priority < prio){
		newNode->next = q->head;
		q->head = newNode;
	}
	else{
		node *start = q->head;
		//traverse list to insert the node according to prio
		while(start->next != NULL && start->next->priority > prio)
			start = start->next;
		newNode->next = start->next;
		start->next = newNode;
	}
	q->numNodes++;
	//send signal to pq_next once items have been inserted into list
	pthread_cond_signal(&q->cv);
}
void *pq_next(pq_t *q){
	//lock until something has been inserted- critical section
	pthread_mutex_lock(&q->mx);
	while(q->numNodes <= 0)
		pthread_cond_wait(&q->cv, &q->mx);
	pthread_mutex_unlock(&q->mx);
	//if there is something in the list, pop off the top priority item and return it
	if(q->head && q->numNodes > 0){
		node* topPrio = q->head;
		q->head = q->head->next;
		void *item = topPrio->value;
		q->numNodes--;
		free(topPrio);
		return item;
	}
	//if it makes it here, list is empty and something went wrong
	perror("Nothing in list");
	return NULL;
}


