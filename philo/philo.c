/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 11:59:58 by jinypark          #+#    #+#             */
/*   Updated: 2022/08/11 12:50:15 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	set_arg(int argc, char **argv, t_info *info)
{
	(*info).arg.philo_n = ft_atoi(argv[1]);
	if ((*info).arg.philo_n < 0)
		return (ERROR);
	(*info).arg.life_t = ft_atoi(argv[2]);
	if ((*info).arg.life_t < 0)
		return (ERROR);
	(*info).arg.eat_t = ft_atoi(argv[3]);
	if ((*info).arg.eat_t < 0)
		return (ERROR);
	(*info).arg.sleep_t = ft_atoi(argv[4]);
	if ((*info).arg.sleep_t < 0)
		return (ERROR);
	if (argc == 6)
	{
		(*info).arg.eat_cnt = ft_atoi(argv[5]);
		if ((*info).arg.eat_cnt < 0)
			return (ERROR);
	}
	return (SUCCESS);
}

int	init_info(t_info *info)
{
	int	i;

	(*info).fork_mutex = malloc(sizeof(pthread_mutex_t) * (*info).arg.philo_n);
	if ((*info).fork_mutex == NULL)
		return (ERROR);
	i = 0;
	while (i < (*info).arg.philo_n)
	{
		if (pthread_mutex_init(&(*info).fork_mutex[i], PTHREAD_MUTEX_NORMAL))
			return (ERROR);
		++i;
	}
	pthread_mutex_init(&(*info).prt_mutex, NULL);
	pthread_mutex_init(&(*info).time_mutex, NULL);
	pthread_mutex_init(&(*info).cnt_mutex, NULL);
	info->flags.eat_f = 0;
	info->flags.die_f = 0;
	info->flags.err_f = 0;
	return (SUCCESS);
}



t_philo	*init_philo(t_info *info)
{
	int		n;
	int		i;
	t_philo	*philo;

	n = (*info).arg.philo_n;
	i = 0;
	philo = malloc((*info).arg.philo_n * sizeof(t_philo));
	while (i < n)
	{
		philo[i].life_time = get_time();
		philo[i].info = info;
		philo[i].idx = i;
		philo[i].p_eat_cnt = 0;
		++i;
	}
	i = 0;
	pthread_mutex_lock(&philo->info->time_mutex);
	while (i < n)
	{
		pthread_create(&philo[i].tid, NULL, &philo_action, &philo[i]);
		++i;
	}
	info->start_time = get_time();
	pthread_mutex_unlock(&philo->info->time_mutex);
	return (philo);
}

int	main(int argc, char *argv[])
{
	t_info	info;
	t_philo	*philo;
	int		i;

	if (!(argc == 5 || argc == 6))
		return (ERROR);
	set_arg(argc, argv, &info);
	init_info(&info);
	philo = init_philo(&info);
	monitor(philo);

	i = 0;
	while (i < info.arg.philo_n)
	{
		pthread_mutex_unlock(&info.fork_mutex[i]);
		++i;
	}
	i = 0;
	while (i < info.arg.philo_n)
	{
		pthread_join(philo[i].tid, NULL);
		++i;
	}
	free(philo);
	free(info.fork_mutex);
	pthread_mutex_unlock(&info.prt_mutex);
	return (0);
}
