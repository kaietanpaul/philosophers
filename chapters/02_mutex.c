#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int	counter = 100000;
pthread_mutex_t	lock;

void	*decrement(void *arg)	///< this function will be called by the thread
{
	while (1)	///< infinite loop
	{
		pthread_mutex_lock(&lock);	///< locking the mutex
		if (counter > 0)	///< checking if the counter is greater than 0
			counter--;	///< decrementing the counter
		else
		{
			pthread_mutex_unlock(&lock);	///< unlocking the mutex
			break;	///< breaking the loop
		}
		pthread_mutex_unlock(&lock);	///< unlocking the mutex
	}
	return (NULL);
}

int	main(void)
{
	pthread_t		thread1, thread2, thread3;	///< this is a struct that lives inside the library

	pthread_mutex_init(&lock, NULL);	///< initializing the mutex

	pthread_create(&thread1, NULL, decrement, NULL);	///< creating a thread, passing the function, and the argument
	pthread_create(&thread2, NULL, decrement, NULL);
	pthread_create(&thread3, NULL, decrement, NULL);

	pthread_join(thread1, NULL);	///< waiting for the thread to finish
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);

	printf("final counter: %d", counter);	///< printing the final value of the counter

	pthread_mutex_destroy(&lock);	///< destroying the mutex

	return (0);
}