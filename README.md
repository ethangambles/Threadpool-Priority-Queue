# Threadpool-Priority-Queue
The first part of this project is implementing a thread-safe, unbounded priority queue. Unbounded means that dynamic memory allocation is necessary so that an unlimited number of items can be added to the queue.
Thread safe means that no race conditions are found in the functions. I was given the 'pq.h' file, and implemented my priority queue in the 'pqimpl.c' file.

The second part of this project is building a threadpool using this priority queue implementation. The threads will remove items from the queue based on priority and execute their tasks. This should be done without race conditions.
I was given the 'threadpool.h' file, and implemented my threadpool in the 'threadpool.c' file.
