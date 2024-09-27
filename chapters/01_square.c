#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void	*square(void *arg)	///< this function will be called by the thread
{
	const int	*int_ptr = (int*)arg;	///< making int pointer from the arg, that was casted from void to int
	const int	num = *int_ptr;	///< dereferencing int pointer to get the value

	int	*result = malloc(sizeof(int));	///< allocating memory for the result of the thread function

	*result = num * num;	///< squaring the number
	return (result);	///< returning the result
}

int	main(void)
{
	pthread_t	thread_1;	///< this is a struct that lives inside the library
	int	num = 12;

	pthread_create(&thread_1, NULL, square, &num);	///< creating a thread, passing the function, and the argument

	int	*result;	///< this will be the result of the thread

	pthread_join(thread_1, (void**)&result);	///< waiting for the thread to finish, and getting the result from the thread function

	printf("%d", *result);

	free(result);	///< freeing the memory that was allocated for the result of the thread function
	return (0);
}