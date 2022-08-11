/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 12:00:07 by jinypark          #+#    #+#             */
/*   Updated: 2022/08/11 12:52:56 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->fork_mutex \
	[((philo->idx + (philo->idx % 2 != 0)) % philo->info->arg.philo_n)]);
	philo_print(philo, philo->idx, "has taken a fork");
	pthread_mutex_lock(&philo->info->fork_mutex \
	[((philo->idx + (philo->idx % 2 == 0)) % philo->info->arg.philo_n)]);
	philo_print(philo, philo->idx, "has taken a fork");
}

void	philo_eat(t_philo *philo)
{
	philo_print(philo, philo->idx, "is eating");
	pthread_mutex_lock(&philo->info->cnt_mutex);
	if (++(philo->p_eat_cnt) == philo->info->arg.eat_cnt)
		philo->info->flags.eat_f++;
	philo->life_time = get_time();
	pthread_mutex_unlock(&philo->info->cnt_mutex);
	smart_timer(philo->info->arg.eat_t);
}

void	philo_sleep(t_philo *philo)
{
	pthread_mutex_unlock(&philo->info->fork_mutex \
	[((philo->idx + (philo->idx % 2 != 0)) % philo->info->arg.philo_n)]);
	pthread_mutex_unlock(&philo->info->fork_mutex \
	[((philo->idx + (philo->idx % 2 == 0)) % philo->info->arg.philo_n)]);
	philo_print(philo, philo->idx, "is sleeping");
	smart_timer(philo->info->arg.sleep_t);
}

void	philo_think(t_philo *philo)
{
	philo_print(philo, philo->idx, "is thinking");
	usleep(20);
}

void	*philo_action(void *param)
{
	t_philo	*philo;
	int		end;

	end = 0;
	philo = (t_philo *)param;
	pthread_mutex_lock(&philo->info->time_mutex);
	philo->life_time = get_time();
	pthread_mutex_unlock(&philo->info->time_mutex);
	if (philo->info->arg.philo_n == 1)
	{
		philo_print(philo, philo->idx, "has taken a fork");
		while (!philo->info->flags.die_f)
			;
		return (NULL);
	}
	if (philo->info->arg.philo_n % 2 == 0 && philo->idx % 2 != 0)
		smart_timer(philo->info->arg.eat_t / 2);
	while (1)
	{
		philo_fork(philo);
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
		pthread_mutex_lock(&philo->info->cnt_mutex);
		if (philo->info->flags.die_f > 0)
			end = 1;
		pthread_mutex_unlock(&philo->info->cnt_mutex);
		if (end == 1)
			return (NULL);
	}
	return (NULL);
}