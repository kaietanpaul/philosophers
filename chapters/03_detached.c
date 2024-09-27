#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	*detached()	///< this function will be called by the thread
{
	printf("Detached working\n");
	sleep(2);	///< sleeping for 2 seconds
	printf("Detached done!\n");
	return (NULL);
}

int	main(void)
{
	pthread_t	T1, T2, T3, T4, T5;		///< this is a struct that lives inside the library

	pthread_create(&T1, NULL, detached, NULL);	///< creating a thread, passing the function, and the argument
	pthread_create(&T2, NULL, detached, NULL);
	pthread_create(&T3, NULL, detached, NULL);
	pthread_create(&T4, NULL, detached, NULL);
	pthread_create(&T5, NULL, detached, NULL);

	pthread_detach(T1);	 ///< detaching the thread
	pthread_detach(T2);
	pthread_detach(T3);
	pthread_detach(T4);
	pthread_detach(T5);

	sleep(2);		///< sleeping for 2 seconds to let the detached threads finish

	return (0);
}