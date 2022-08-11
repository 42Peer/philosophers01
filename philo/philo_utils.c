/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 12:00:03 by jinypark          #+#    #+#             */
/*   Updated: 2022/08/11 12:53:57 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	smart_timer(size_t time)
{
	size_t	start;

	start = get_time();
	while (get_time() - start < time)
		usleep(100);
}

void	philo_print(t_philo *philo, int idx, char *status)
{
	int	end_flag;

	end_flag = 0;
	pthread_mutex_lock(&philo->info->cnt_mutex);
	if (philo->info->flags.die_f > 0)
		end_flag = 1;
	pthread_mutex_unlock(&philo->info->cnt_mutex);
	if (end_flag == 1)
		return ;
	pthread_mutex_lock(&philo->info->prt_mutex);
	printf("%ld %d %s\n", get_time() - philo->info->start_time, idx + 1, status);
	pthread_mutex_unlock(&philo->info->prt_mutex);
}