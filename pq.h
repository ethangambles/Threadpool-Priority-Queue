#ifndef PQ_H
#define PQ_H
#include <pthread.h>

/*Interface to a very simple *unbounded* thread-safe priority queue.
 *Priority is a signed number; items enqueued with higher prio values are
 *returned before those with lower values. (Negative values are fine.)
 *Items with the same priority are  returned in the order of insertion (FIFO).
 *There is no safe way to destroy a priority queue once it is created,
 *so THIS CODE IS NOT FOR PRODUCTION USE.
 *       */
typedef struct{
	pthread_mutex_t mx;
	pthread_cond_t cv;
	struct node *head;
	int numNodes;
}pq_t;

typedef struct node{
	void* value;
	short priority;
	struct node *next;
	struct node *head;
}node;

// create and initialize a prio queue instance. Return NULL on error.
pq_t *pq_create(void);

// /* This operation never blocks.  Fails (aborts) if OS runs out of memory.
//* N.B. the second argument is the "value" to be stored at the given prio.
//* It will eventually be returned by pq_next() unless it never becomes the
//* highest priority item.  It can be a pointer to anything, or even
//* an integer type.
void pq_insert(pq_t *q, void *item, short prio);

// Return the oldest item with highest priority.
//* In other words, the item returns satisfies the predicate:
//* No item in the queue has higher priority AND any item with the same
//* priority was inserted after this item.
//* The calling thread will block if the queue is empty.
void *pq_next(pq_t *q); // returns the oldest item with highest-priority

#endif
