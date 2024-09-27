#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	counter1 = 0;
int	counter2 = 0;
int	x = 500;
int	times1 = 0;
int	times2 = 0;

pthread_mutex_t MTX1, MTX2;

void	*increment1(void *arg)
{
	char *ch = (char*)arg;

	while (1)
	{
		pthread_mutex_lock(&MTX1);
		if (times1 < x)
		{
			printf("%s: ++ %d\n", ch, counter1);
			times1 ++;
			counter1++;
		}
		else
		{
			pthread_mutex_unlock(&MTX1);
			break;
		}
		pthread_mutex_unlock(&MTX1);
	}
	return (NULL);
}
void	*increment2(void *arg)
{
	char *ch = (char*)arg;

	while (1)
	{
		pthread_mutex_lock(&MTX2);
		if (times2 < x)
		{
			printf("%s: ++ %d\n", ch, counter2);
			times2 ++;
			counter2++;
		}
		else
		{
			pthread_mutex_unlock(&MTX2);
			break;
		}
		pthread_mutex_unlock(&MTX2);
	}
	return (NULL);
}

void	*decrement(void *arg)
{
	char *ch = (char*)arg;
	int	i = 0;

	while (1)
	{
		pthread_mutex_lock(&MTX1);
		pthread_mutex_lock(&MTX2);
		if (i < x)
		{
			printf("[%s: -- %d]\n", ch, counter1);
			printf("[%s: -- %d]\n", ch, counter2);
			counter1--;
			counter2--;
			i++;
		}
		else
		{
			pthread_mutex_unlock(&MTX1);
			pthread_mutex_unlock(&MTX2);
			break;
		}
		pthread_mutex_unlock(&MTX1);
		pthread_mutex_unlock(&MTX2);
	}
	return (NULL);
}

int	main(void)
{
	pthread_t		thread1, thread2, thread3;	///< this is a struct that lives inside the library

	pthread_mutex_init(&MTX1, NULL);	///< initializing the mutex
	pthread_mutex_init(&MTX2, NULL);	///< initializing the mutex

	pthread_create(&thread1, NULL, increment1, "T1");	///< creating a thread, passing the function, and the argument
	pthread_create(&thread2, NULL, increment2, "T2");
	pthread_create(&thread3, NULL, decrement, "T3");

	pthread_join(thread1, NULL);	///< waiting for the thread to finish
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);

	printf("final counter1: %d\n", counter1);	///< printing the final value of the counter
	printf("final counter2: %d", counter2);	///< printing the final value of the counter

	pthread_mutex_destroy(&MTX1);	///< destroying the mutex
	pthread_mutex_destroy(&MTX2);	///< destroying the mutex

	return (0);
}