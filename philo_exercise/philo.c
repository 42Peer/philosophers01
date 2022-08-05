/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 13:08:11 by jinypark          #+#    #+#             */
/*   Updated: 2022/08/05 21:18:38 by jinypark         ###   ########.fr       */
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

void	philo_printf(int sec, int idx, char *str, pthread_mutex_t *print_mutex, t_philo *philo)
{
	struct timeval t;

	pthread_mutex_lock(print_mutex);
	// if (strstr(str, "sleeping")) //수정
	// 	philo->life_time = philo->arg.life_t;
	gettimeofday(&t, NULL);
	printf("%d %d %s\n", t.tv_usec, idx, str);
	if (strstr(str, "died")) //수정
		return ;
	pthread_mutex_unlock(print_mutex);
}

int	init_info(t_info *info)
{
	int	i;

	(*info).philo = malloc(sizeof(t_philo) * (*info).arg.philo_n);
	if ((*info).philo == NULL)
		return (ERROR);
	(*info).mutex = malloc(sizeof(pthread_mutex_t) * (*info).arg.philo_n);
	if ((*info).mutex == NULL)
		return (ERROR);
	i = 0;
	while (i < (*info).arg.philo_n)
	{
		if (pthread_mutex_init(&(*info).mutex[i], PTHREAD_MUTEX_NORMAL))
			return (ERROR);
		++i;
	}
	return (SUCCESS);
}

void	philo_fork(t_philo *philo)
{
	int		idx;
	struct	timeval t;

	idx = philo->idx;
	if (idx % 2 == 0)
	{
		pthread_mutex_lock(&philo->mutex[idx]);
		gettimeofday(&t, NULL);
		philo_printf(t.tv_usec, idx + 1, "has taken a fork", philo->p_mutex, philo);
		pthread_mutex_lock(&philo->mutex[(idx + 1) % philo->arg.philo_n]);
		philo_printf(t.tv_usec, idx + 1, "has taken a fork", philo->p_mutex, philo);
	}
	else
	{
		pthread_mutex_lock(&philo->mutex[(idx + 1) % philo->arg.philo_n]);
		gettimeofday(&t, NULL);
		philo_printf(t.tv_usec, idx + 1, "has taken a fork", philo->p_mutex, philo);
		pthread_mutex_lock(&philo->mutex[idx]);
		philo_printf(t.tv_usec, idx + 1, "has taken a fork", philo->p_mutex, philo);
	}
}

void	philo_eat(t_philo *philo)
{
	int	idx;
	struct	timeval t;

	idx = philo->idx;
	philo->life_time = philo->arg.life_t;
	gettimeofday(&t, NULL);
	philo_printf(t.tv_usec, idx + 1, "is eating", philo->p_mutex, philo);
	usleep(philo->arg.eat_t);
}

void	philo_sleep(t_philo *philo)
{
	int 	idx;
	struct	timeval t;

	idx = philo->idx;
	// philo->life_time = philo->arg.life_t;
	pthread_mutex_unlock(&philo->mutex[idx]);
	pthread_mutex_unlock(&philo->mutex[(idx + 1) % philo->arg.philo_n]);
	gettimeofday(&t, NULL);
	philo_printf(t.tv_usec, idx + 1, "is sleeping", philo->p_mutex, philo);
	usleep(philo->arg.sleep_t);
}

void	philo_think(t_philo *philo)
{
	int		idx;
	struct	timeval t;

	idx = philo->idx;
	gettimeofday(&t, NULL);
	philo_printf(t.tv_usec, idx + 1, "is thinking", philo->p_mutex, philo);
}

void	*philo_init(void *param)
{
	t_philo	*philo;

	philo = (t_philo *)param;
	while (1)
	{
		philo_fork(philo);
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_info			info;
	int				i;
	pthread_mutex_t p_mutex;
	struct	timeval t;

	if (!(argc == 5 || argc == 6))
		return (0);
	set_arg(argc, argv, &info);
	if (init_info(&info) == ERROR)
		return (0);
	pthread_mutex_init(&p_mutex, NULL);
	i = 0;
	while (i < info.arg.philo_n)
	{
		info.philo[i].idx = i;
		info.philo[i].mutex = info.mutex;
		info.philo[i].p_mutex = &p_mutex;
		info.philo[i].arg = info.arg;
		pthread_create(&(info.philo[i].tid), NULL, &philo_init, (void *)&(info.philo[i]));
		++i;
	}
	while (1)
	{
		i = 0;
		while (i < info.arg.philo_n)
		{
			info.philo[i].life_time--;
			if (info.philo[i].life_time == 0)
			{
				gettimeofday(&t, NULL);
				philo_printf(t.tv_usec, (i + 1) % info.arg.philo_n + 1, "died", &p_mutex, NULL);
				i = 0;
				// while (i < info.arg.philo_n)
				// {
				// 	// pthread_join(info.philo[i].tid, NULL);
				// 	pthread_detach(info.philo[i].tid);
				// 	++i;
				// }
				return (0);
			}
			++i;
		}
		usleep(1);
	}
	// i = 0;
	// while (i < info.arg.philo_n)
	// {
	// 	pthread_join(info.philo[i].tid, NULL);
	// 	++i;
	// }
	return (0);
}


// [philo-Test #1]: Given 4 310 200 100 arguments to philo, a philosopher should die !
// [philo-Test #4]: Given 4 410 200 200 7 arguments to philo, philo should stop !
// [philo-Test #4]: Given 4 410 200 200 12 arguments to philo, philo should stop !
// [philo-Test #4]: Given 4 410 200 200 15 arguments to philo, philo should stop !
// [philo-Test #5]: Given 2 60 60 60 arguments to philo, a philosopher should die !