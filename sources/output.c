/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygurma <ygurma@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 14:04:23 by ygurma            #+#    #+#             */
/*   Updated: 2023/12/11 14:04:29 by ygurma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* print_status_debug:
*	Prints the philosopher's status in an easier to read,
*	colorful format to help with debugging. For fork-taking
*	statuses, extra information is displayed to show which fork
*	the philosopher has taken.
*/
static void	print_status_debug(t_philo *philo, char *color,
								char *str, t_status status)
{
	if (status == GOT_FORK_1)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork [%d]\n",
			get_time_in_ms() - philo->table->start_time,
			color, philo->id + 1, str, philo->fork[0]);
	else if (status == GOT_FORK_2)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork [%d]\n",
			get_time_in_ms() - philo->table->start_time,
			color, philo->id + 1, str, philo->fork[1]);
	else
		printf("[%10ld]\t%s%03d\t%s\e[0m\n",
			get_time_in_ms() - philo->table->start_time,
			color, philo->id + 1, str);
}

static void	write_status_debug(t_philo *philo, t_status status)
{
	if (status == DIED)
		print_status_debug(philo, RED, "died", status);
	else if (status == EATING)
		print_status_debug(philo, GREEN, "is eating", status);
	else if (status == SLEEPING)
		print_status_debug(philo, CYAN, "is sleeping", status);
	else if (status == THINKING)
		print_status_debug(philo, CYAN, "is thinking", status);
	else if (status == GOT_FORK_1)
		print_status_debug(philo, PURPLE, "has taken a fork", status);
	else if (status == GOT_FORK_2)
		print_status_debug(philo, PURPLE, "has taken a fork", status);
}

static void	print_status(t_philo *philo, char *str)
{
	printf("%1ld %d %s\n", get_time_in_ms() - philo->table->start_time,
		philo->id + 1, str);
}

void	write_status(t_philo *philo, bool reaper_report, t_status status)
{
	pthread_mutex_lock(&philo->table->write_lock);
	if (has_simulation_stopped(philo->table) == true && reaper_report == false)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (DEBUG_FORMATTING == true)
	{
		write_status_debug(philo, status);
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (status == DIED)
		print_status(philo, "died");
	else if (status == EATING)
		print_status(philo, "is eating");
	else if (status == SLEEPING)
		print_status(philo, "is sleeping");
	else if (status == THINKING)
		print_status(philo, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->table->write_lock);
}

void	write_outcome(t_table *table)
{
	unsigned int	i;
	unsigned int	full_count;

	full_count = 0;
	i = 0;
	while (i < table->nb_philos)
	{
		if (table->philos[i]->times_ate >= (unsigned int)table->must_eat_count)
			full_count++;
		i++;
	}
	pthread_mutex_lock(&table->write_lock);
	printf("%d/%d philosophers had at least %d meals.\n",
		full_count, table->nb_philos, table->must_eat_count);
	pthread_mutex_unlock(&table->write_lock);
	return ;
}
