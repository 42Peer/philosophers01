/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 12:20:59 by jinypark          #+#    #+#             */
/*   Updated: 2022/08/11 12:53:30 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_dieat(t_philo *philo)
{
	philo->info->flags.die_f = 1;
	pthread_mutex_lock(&philo->info->prt_mutex);
	printf("eat_f end here\n");
}


void	philo_die(t_philo *philo, int idx)
{
	philo->info->flags.die_f = 1;
	pthread_mutex_lock(&philo->info->prt_mutex);
	printf("%ld %d died\n", get_time() - philo->info->start_time, idx + 1);
	pthread_mutex_unlock(&philo->info->fork_mutex[idx]);
	pthread_mutex_unlock(&philo->info->fork_mutex \
	[(idx + 1) % philo->info->arg.philo_n]);
}

void monitor(t_philo *philo)
{
	int		i;
	int		end;

	end = 0;
	i = 0;
	while (1)
	{
		pthread_mutex_lock(&philo->info->time_mutex);
		if (get_time() - philo[i].life_time > philo->info->arg.life_t)
		{
			end = 1;
			philo_die(philo, i);
		}
		pthread_mutex_unlock(&philo->info->time_mutex);
		pthread_mutex_lock(&philo->info->cnt_mutex);
		if (philo->info->flags.eat_f >= philo->info->arg.philo_n)
		{
			end = 1;
			philo_dieat(philo);
		}
		pthread_mutex_unlock(&philo->info->cnt_mutex);
		if (end)
			break ;
		i = (i + 1) % philo->info->arg.philo_n;
	}
}