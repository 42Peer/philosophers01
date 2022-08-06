#include "philo.h"

size_t	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return(time.tv_sec * 1000000 + time.tv_usec);
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
		usleep(500);
	}
}

int	set_arg(int argc, char **argv, t_info *info)
{
	(*info).arg.philo_n =ft_atoi(argv[1]);
	if ((*info).arg.philo_n < 0)
		return (ERROR);
	(*info).arg.life_t = ft_atoi(argv[2]) * 1000;
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

void	philo_print(t_info *info, int idx, char *status)
{
	struct timeval	t;

	pthread_mutex_lock(&info->prt_mutex);
	// gettimeofday(&t, NULL);
	printf("%ld %d %s\n", get_time() - info->start_time, idx, status);
	// if (info->flags.err_f > 0
	// || info->flags.die_f > 0)
	// 	return ;
	if (strstr(status, "died")) // ---------------> need to change to ft_stsstr
		return ;
	pthread_mutex_unlock(&info->prt_mutex);
		//|| info->flags.eat_f >= info->arg.philo_n)

}

void	philo_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->fork_mutex[((philo->idx + (philo->idx % 2 != 0)) % philo->info->arg.philo_n)]);
	philo_print(philo->info, philo->idx, "has taken a fork");
	pthread_mutex_lock(&philo->info->fork_mutex[((philo->idx + (philo->idx % 2 == 0)) % philo->info->arg.philo_n)]);
	philo_print(philo->info, philo->idx, "has taken a fork");
}

void	philo_eat(t_philo *philo)
{
	struct timeval t;

	philo_print(philo->info, philo->idx, "is eating");
//	if (philo->info->flags.die_f == 1)
//		philo->life_time = gettimeofday(&t, NULL);
	philo->life_time = get_time();
	smart_timer(philo, philo->info->arg.eat_t * 1000);
	(*philo).p_eat_cnt++;
	if (philo->p_eat_cnt == philo->info->arg.eat_cnt)
		philo->info->flags.eat_f++;
}

void	philo_sleep(t_philo *philo)
{
	pthread_mutex_unlock(&philo->info->fork_mutex[((philo->idx + (philo->idx % 2 == 0)) % philo->info->arg.philo_n)]);
	pthread_mutex_unlock(&philo->info->fork_mutex[((philo->idx + (philo->idx % 2 != 0)) % philo->info->arg.philo_n)]);
	philo_print(philo->info, philo->idx, "is sleeping");
	smart_timer(philo, philo->info->arg.sleep_t * 1000);
}

void	philo_think(t_philo *philo)
{
	philo_print(philo->info, philo->idx, "is thinking");
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
	while (1)
	{
		// if (philo->info->flags.die_f == 0)
		// {
		// 	philo->life_time = gettimeofday(&t, NULL);
		// 	philo->info->flags.die_f = 1;
		// }
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
		// 주석 : 순서에 따라 죽는거 영향을 끼침 
		++i;
	}
	// pthread_create(&philo[i].tid, NULL, &philo_action, &philo[i]);
	
	return (philo);
}

//void debug()
//{
//	system("leaks philo");
//}

int	main(int argc, char *argv[])
{
	//atexit(debug);
	t_info	info;
	t_philo	*philo;

	if (!(argc == 5 || argc == 6))
		return (ERROR);

	set_arg(argc, argv, &info);
	init_info(&info);
	philo = init_philo(&info);
	while(1)
	{
		int j = 4; // 주석 : 테스트를 위해 4부터 시작 
		while (j--)
		{
			//printf("life %d: %zu\n", j, get_time() - philo[j].life_time);
			if (get_time() - philo[j].life_time >= info.arg.life_t)
			{
				// info.flags.die_f = 1;
				philo_print(&info, j, "died");
				//j = 4;
				//while(j--)
					// pthread_mutex_destroy(&info.fork_mutex[j]);
				exit(1);
			}
			// --philo[j].life_time;
			// if (philo[j].life_time == 0)
			// {
			// 	philo_print(&info, j, "died");
			// 	//j = 4;
			// 	//while(j--)
			// 		// pthread_mutex_destroy(&info.fork_mutex[j]);
			// 	exit(1);
				
			// 	return (0);
			// }
			// if (j == 0 || j ==2)
			// {
			// 	printf("life 0: %zu\n", philo[0].life_time);
			// 	printf("life 2: %zu\n", philo[2].life_time);
			// }
			// ++j;
		}
		// if (info.flags.eat_f == info.arg.philo_n)
		// {
		// 	//philo_print(&info, 0, "HEY THIS IS THE END\n");
		// 	j = 4;
		// 	//pthread_mutex_lock(&info.prt_mutex);
		// 	//while(j--)
		// 	//	pthread_mutex_destroy(&info.fork_mutex[j]);
		// 	return (0);
		// }
		//if (info.flags.die_f > 0)
		//{
		//	philo_print(&info, info.flags.die_f + 1, "IS DIED");
		//	exit(1);
		//}
		// if (info.flags.err_f > 0)
		// 	exit(1);
	}
	return (0);
}

//detach 되었을 때, mutex안됨
//다 죽은 다음에 해야함
//근데 다시 확인은 해봐야함
 // 주전부리 : 젤리 초코렛 과자?
//0 0 has taken a fork
//0 0 has taken a fork
//0 0 is eating
//0 1 has taken a fork
//200 0 is sleeping
//200 1 has taken a fork
//200 1 is eating
//200 3 has taken a fork
//200 3 has taken a fork
//200 3 is eating
//300 0 is thinking
//312 2 died