#include "philo.h"

size_t	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return(time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	smart_timer(t_philo *philo, size_t time)
{
	size_t	start;
	size_t	diff;

	start = get_time();
	while (get_time() - start < time)
	{
		// if (get_time() - philo->life_time > philo->info->arg.life_t)
		// {
		// 	philo->info->flags.die_f = 1;
		// 	return ;
		// }
		usleep(200);
	}
}

int	set_arg(int argc, char **argv, t_info *info)
{
	(*info).arg.philo_n =ft_atoi(argv[1]);
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

void	philo_print_die(t_info *info, int idx, char *status)
{
	pthread_mutex_lock(&info->prt_mutex);
	printf("%ld %d %s\n", get_time() - info->start_time, idx + 1, status);
	pthread_mutex_unlock(&info->prt_mutex);
}

void	philo_print(t_philo *philo, int idx, int status)
{
	struct timeval	t;

	pthread_mutex_lock(&philo->info->prt_mutex);
	if (!philo->info->flags.die_f)
	{
		if (status == FORK)
			printf("%ld %d has taken a fork\n", get_time() - philo->info->start_time, idx + 1);
		else if (status == EATING)
		{
			printf("%ld %d is eating\n", get_time() - philo->info->start_time, idx + 1);
			if (++(philo->p_eat_cnt) == philo->info->arg.eat_cnt)
				philo->info->flags.eat_f++;
		}
		else if (status == SLEEPING)
			printf("%ld %d is sleeping\n", get_time() - philo->info->start_time, idx + 1);
		else if (status == THINKING)
			printf("%ld %d is thinking\n", get_time() - philo->info->start_time, idx + 1);
	}
	pthread_mutex_unlock(&philo->info->prt_mutex);
}

void	philo_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->fork_mutex[((philo->idx + (philo->idx % 2 != 0)) % philo->info->arg.philo_n)]);
	philo_print(philo, philo->idx, FORK);
	pthread_mutex_lock(&philo->info->fork_mutex[((philo->idx + (philo->idx % 2 == 0)) % philo->info->arg.philo_n)]);
	philo_print(philo, philo->idx, FORK);
}

void	philo_eat(t_philo *philo)
{
	struct timeval t;

	philo_print(philo, philo->idx, EATING);
	// (*philo).p_eat_cnt++;
	if (philo->info->flags.eat_f == philo->info->arg.philo_n)
		exit(1);
	philo->life_time = get_time();
	smart_timer(philo, philo->info->arg.eat_t);
}

void	philo_sleep(t_philo *philo)
{
	pthread_mutex_unlock(&philo->info->fork_mutex[((philo->idx + (philo->idx % 2 == 0)) % philo->info->arg.philo_n)]);
	pthread_mutex_unlock(&philo->info->fork_mutex[((philo->idx + (philo->idx % 2 != 0)) % philo->info->arg.philo_n)]);
	philo_print(philo, philo->idx, SLEEPING);
	smart_timer(philo, philo->info->arg.sleep_t);
}

void	philo_think(t_philo *philo)
{
	philo_print(philo, philo->idx, THINKING);
	usleep(50);
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
	info->start_time = get_time();
	info->flags.eat_f = 0;
	info->flags.die_f = 0;
	info->flags.err_f = 0;
	return (SUCCESS);
}

void	*philo_action(void *param)
{
	t_philo *philo;
	struct timeval t;

	philo = (t_philo *)param;
	philo->life_time = get_time();
	while (!philo->info->flags.die_f)
	{
		philo_fork(philo);
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
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
		philo[i].idx = i;
		philo[i].info = info;
		philo[i].p_eat_cnt = 0;
		++i;
	}
	i = 0;
	while (i < n)
	{
		pthread_create(&philo[i].tid, NULL, &philo_action, &philo[i]);
		++i;
	}
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
	while(1)
	{
		i = 0;
		while (i < info.arg.philo_n)
		{
			if (get_time() - philo[i].life_time >= info.arg.life_t)
			{
				info.flags.die_f = 1;
				philo_print_die(&info, i, "died");
				i = 0;
				while (i < info.arg.philo_n)
				{
					pthread_join(philo[i].tid, NULL);
					++i;
				}
				break;
			}
			if (info.flags.eat_f >= info.arg.philo_n)
			{
				exit(1);
				i = 0;
				info.flags.die_f = 1;
				while (i < info.arg.philo_n)
				{
					pthread_join(philo[i].tid, NULL);
					++i;
				}
				break ;
			}
			++i;
		}
		if (info.flags.die_f)
			break ;
	}
	free(philo);
	free(info.fork_mutex);
	return (0);
}

//detach 되었을 때, mutex안됨
//다 죽은 다음에 해야함
//근데 다시 확인은 해봐야함