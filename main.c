#include <pthread.h>	///< this is the library that contains the thread functions
#include <stdio.h>		///< this is the library that contains the printf function
#include <stdlib.h>		///< this is the library that contains the malloc function
#include <unistd.h>		///< this is the library that contains the sleep function

typedef struct		s_consts
{
	int				number_of_philosophers;
}					t_consts;

typedef struct		s_forks
{
	pthread_mutex_t	fork;
	int				fork_id;
}					t_forks;

typedef struct		s_philosopher	///< this is a struct that will be passed as an argument to the thread function
{
	int				philo_id;
	pthread_t		thread;
	// pthread_mutex_t	right_fork;
	t_consts		*consts;
	t_forks			*forks;
}					t_philosopher;	///< this is the name of the struct

void	*philo_print(void *arg)	///< this function will be called by the thread
{
	sleep(3);
	t_philosopher *philosopher = arg;	///< casting the argument to the struct

	int	philo_id = philosopher->philo_id;	///< getting the philosopher id from the struct
	int	philo_count = philosopher->consts->number_of_philosophers;

	while (1)
	{
		//eat
		if (philo_id == 0)
		{
			pthread_mutex_lock(&philosopher->forks[philo_count - 1].fork);
			printf("Philo: %d	picks L fork: %d	%p\n", philo_id, philo_count - 1, (void *)&philosopher->forks[philo_count - 1].fork);
			pthread_mutex_lock(&philosopher->forks[philo_id].fork);
			printf("Philo: %d	picks R fork: %d	%p\n", philo_id, philo_id, (void *)&philosopher->forks[philo_id].fork);
		}
		else
		{
			pthread_mutex_lock(&philosopher->forks[philo_id - 1].fork);
			printf("Philo: %d	picks L fork: %d	%p\n", philo_id, philo_id - 1, (void *)&philosopher->forks[philo_id - 1].fork);
			pthread_mutex_lock(&philosopher->forks[philo_id].fork);
			printf("Philo: %d	picks R fork: %d	%p\n", philo_id, philo_id, (void *)&philosopher->forks[philo_id].fork);
		}
		printf("\nPhilo: %d	EATS\n\n", philo_id);

		//put the fork down
		sleep(0);
		if (philo_id == 0)
		{
			pthread_mutex_unlock(&philosopher->forks[philo_count - 1].fork);
			pthread_mutex_unlock(&philosopher->forks[philo_id].fork);
		}
		else
		{
			pthread_mutex_unlock(&philosopher->forks[philo_id - 1].fork);
			pthread_mutex_unlock(&philosopher->forks[philo_id].fork);
		}
		sleep(0);

		//sleep
		sleep(2);
	}
	return (NULL);
}

void	init(t_philosopher **philosopher, int number_of_philosophers, t_consts *consts, t_forks *forks)
{
	int i = 0;
	while (i < number_of_philosophers) ///< allocating memory for the struct
	{
		philosopher[i] = malloc(sizeof(t_philosopher));
		if (!philosopher[i])
			exit(1);
		philosopher[i]->philo_id = i;	///< setting the philosopher id
		philosopher[i]->consts = consts;	///< setting the struct that contains the number of philosophers
		philosopher[i]->forks = forks;

		philosopher[i]->forks->fork_id = i; // Set the fork id
		pthread_mutex_init(&philosopher[i]->forks->fork, NULL);	///< initializing the mutex
		printf("id:%d addr:%p\n",philosopher[i]->philo_id , &philosopher[i]->forks->fork);
		pthread_create(&philosopher[i]->thread, NULL, philo_print, philosopher[i]);
		///^ creating a thread, passing the function, and the argument
		// sleep(0); ///< OS will switch to another thread if possible and will give the CPU to another thread
		i++;
	}
	i = 0;

	while (i < number_of_philosophers) ///< waiting for the threads to finish
	{
		pthread_join(philosopher[i]->thread, NULL);
		i++;
	}
}

void	free_destroy(t_philosopher **philosopher)
{
	int	i = 0;
	int	philo_count = philosopher[0]->consts->number_of_philosophers;
	while (i < philo_count)	///< freeing the memory that was allocated for the struct
	{
		pthread_mutex_destroy(&philosopher[i]->forks->fork);
		free(philosopher[i]->forks);
		free(philosopher[i]);
		i++;
	}

	free(philosopher);	///< freeing the memory that was allocated for the array of pointers

}

int	main(int ac, char **av)
{
	int				number_of_philosophers = (int)strtol(av[1], NULL, 10);
	// t_philosopher	*philosopher[number_of_philosophers];	///< this is an array of pointers to the struct
	/// dynamic allocation allows for allocating when the program runs, instead of when it compiles
	t_philosopher	**philosopher = malloc(sizeof(t_philosopher *) * number_of_philosophers);
	if (!philosopher)
		exit(1);
	t_forks			*forks = malloc(sizeof(t_forks) * number_of_philosophers);
	if (!forks)
		exit(1);
	t_consts	consts;
	consts.number_of_philosophers = number_of_philosophers;

	init(philosopher, number_of_philosophers, &consts, forks);
	free_destroy(philosopher);

	return (0);
}
