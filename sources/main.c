/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygurma <ygurma@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 14:21:16 by ygurma            #+#    #+#             */
/*   Updated: 2023/12/11 14:21:26 by ygurma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* start_simulation:
*	Launches the simulation by creating a grim reaper thread as well as
*	one thread for each philosopher.
*	Returns true if the simulation was successfully started, false if there
*	was an error. 
*/
static bool	start_simulation(t_table *table)
{
	unsigned int	i;

	table->start_time = get_time_in_ms() + (table->nb_philos * 2 * 10);
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_create(&table->philos[i]->thread, NULL,
				&philosopher, table->philos[i]) != 0)
			return (error_failure(STR_ERR_THREAD, NULL, table));
		i++;
	}
	if (table->nb_philos > 1)
	{
		if (pthread_create(&table->grim_reaper, NULL,
				&grim_reaper, table) != 0)
			return (error_failure(STR_ERR_THREAD, NULL, table));
	}
	return (true);
}

/* stop_simulation:
*	Waits for all threads to be joined then destroys mutexes and frees
*	allocated memory.
*/
static void	stop_simulation(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_join(table->philos[i]->thread, NULL);
		i++;
	}
	if (table->nb_philos > 1)
		pthread_join(table->grim_reaper, NULL);
	if (DEBUG_FORMATTING == true && table->must_eat_count != -1)
		write_outcome(table);
	destroy_mutexes(table);
	free_table(table);
}

int	main(int ac, char **av)
{
	t_table	*table;

	table = NULL;
	if (ac - 1 < 4 || ac - 1 > 5)
		return (msg(STR_USAGE, NULL, EXIT_FAILURE));
	if (!is_valid_input(ac, av))
		return (EXIT_FAILURE);
	table = init_table(ac, av, 1);
	if (!table)
		return (EXIT_FAILURE);
	if (!start_simulation(table))
		return (EXIT_FAILURE);
	stop_simulation(table);
	return (EXIT_SUCCESS);
}
