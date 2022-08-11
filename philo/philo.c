#include "philo.h"

size_t	get_time()
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return(time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	smart_timer(size_t time)
{
	size_t	start;

	start = get_time();
	while (get_time() - start < time)
		usleep(100);
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

void	philo_print(t_philo *philo, int idx, char *status)
{
	int	end_flag;

	end_flag = 0;
	pthread_mutex_lock(&philo->info->t_mutex);
	if (philo->info->flags.die_f > 0)
		end_flag = 1;
	pthread_mutex_unlock(&philo->info->t_mutex);
	if (end_flag == 1)
		return ;
	pthread_mutex_lock(&philo->info->prt_mutex);
	printf("%ld %d %s\n", get_time() - philo->info->start_time, idx + 1, status);
	pthread_mutex_unlock(&philo->info->prt_mutex);
}

void	philo_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->fork_mutex[((philo->idx + (philo->idx % 2 != 0)) % philo->info->arg.philo_n)]);
	philo_print(philo, philo->idx, "has taken a fork");
	pthread_mutex_lock(&philo->info->fork_mutex[((philo->idx + (philo->idx % 2 == 0)) % philo->info->arg.philo_n)]);
	philo_print(philo, philo->idx, "has taken a fork");
}

void	philo_eat(t_philo *philo)
{
	philo_print(philo, philo->idx, "is eating");
	pthread_mutex_lock(&philo->info->t_mutex);
	if (++(philo->p_eat_cnt) == philo->info->arg.eat_cnt)
		philo->info->flags.eat_f++;
	philo->life_time = get_time();
	pthread_mutex_unlock(&philo->info->t_mutex);
	smart_timer(philo->info->arg.eat_t);
}

void	philo_sleep(t_philo *philo)
{
	pthread_mutex_unlock(&philo->info->fork_mutex[((philo->idx + (philo->idx % 2 != 0)) % philo->info->arg.philo_n)]);
	pthread_mutex_unlock(&philo->info->fork_mutex[((philo->idx + (philo->idx % 2 == 0)) % philo->info->arg.philo_n)]);
	philo_print(philo, philo->idx, "is sleeping");
	smart_timer(philo->info->arg.sleep_t);
}

void	philo_think(t_philo *philo)
{
	philo_print(philo, philo->idx, "is thinking");
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
	pthread_mutex_init(&(*info).t_mutex, NULL);
	info->flags.eat_f = 0;
	info->flags.die_f = 0;
	info->flags.err_f = 0;
	return (SUCCESS);
}

void	*philo_action(void *param)
{
	t_philo *philo;
	int		end;

	end  = 0;
	philo = (t_philo *)param;
	pthread_mutex_lock(&philo->info->t_mutex);
	philo->life_time = get_time();
	pthread_mutex_unlock(&philo->info->t_mutex);
	if (philo->info->arg.philo_n == 1)
	{
		philo_print(philo, philo->idx, "has taken a fork");
		pthread_mutex_unlock(&philo->info->t_mutex);
		while (!philo->info->flags.die_f);
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
		pthread_mutex_lock(&philo->info->t_mutex);
		if (philo->info->flags.die_f > 0)
			end = 1;
		pthread_mutex_unlock(&philo->info->t_mutex);
		if (end == 1)
			return (NULL);
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
		philo[i].info = info;
		philo[i].idx = i;
		philo[i].p_eat_cnt = 0;
		++i;
	}
	i = 0;
	philo->info->start_time = get_time();
	pthread_mutex_lock(&info->t_mutex);
	while (i < n)
	{
		pthread_create(&philo[i].tid, NULL, &philo_action, &philo[i]);
		++i;
	}
	pthread_mutex_unlock(&info->t_mutex);
	return (philo);
}

void monitor(t_philo *philo)
{
	int		i;
	int		end;
	// t_info	info;

	end = 0;
	// info = *(*philo).info;
	while(1)
	{
		i = 0;
		while (i < philo->info->arg.philo_n)
		{
			pthread_mutex_lock(&philo->info->t_mutex);
			if (get_time() - philo[i].life_time > philo->info->arg.life_t)
				end = 1;
			pthread_mutex_unlock(&philo->info->t_mutex);
			if (end == 1)
			{
				pthread_mutex_lock(&philo->info->prt_mutex);
				philo->info->flags.die_f = 1;
				printf("%ld %d died\n" , get_time() - philo->info->start_time, i + 1);
				pthread_mutex_unlock(&philo->info->fork_mutex[i]);
				pthread_mutex_unlock(&philo->info->fork_mutex[(i + 1) % philo->info->arg.philo_n]);
				// pthread_mutex_unlock(&philo->info->t_mutex);
				break;
			}
			pthread_mutex_lock(&philo->info->t_mutex);
			if (philo->info->flags.eat_f >= philo->info->arg.philo_n)
				end = 2;
			pthread_mutex_unlock(&philo->info->t_mutex);
			if (end == 2)
			{
				pthread_mutex_lock(&philo->info->prt_mutex);
				philo->info->flags.die_f = 1;
				printf("eat_f end here\n");
				// pthread_mutex_unlock(&philo->info->t_mutex);
				break ;
			}
			// pthread_mutex_unlock(&philo->info->t_mutex);
			++i;

		}
		pthread_mutex_lock(&philo->info->t_mutex);
		if (philo->info->flags.die_f > 0)
			end = 1;
		pthread_mutex_unlock(&philo->info->t_mutex);
		if (end > 0)
			break ;
	}
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
	pthread_mutex_unlock(&info.prt_mutex);
	while (i < info.arg.philo_n)
	{
		pthread_join(philo[i].tid, NULL);
		++i;
	}
	free(philo);
	free(info.fork_mutex);
	return (0);
}
