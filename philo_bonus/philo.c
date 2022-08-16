#include "philo.h"

size_t	get_time()
{
	struct timeval	time;

	if(gettimeofday(&time, NULL) == ERROR)
		return (ERROR);
	return(time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	smart_timer(size_t time)
{
	size_t	start;

	start = get_time();
	// if (start == ERROR)
		// return (ERROR);
	while (get_time() - start < time)
		usleep(100);
}

void	philo_print(t_philo *philo, t_info *info, int idx, char *str)
{
	pthread_mutex_lock(&info->mutex.print);
	if (info->stat.end == 0)
		printf("%ld %d %s\n", get_time() - info->birth_t, idx + 1, str);
	if (strstr(str, "is eating"))
	{
		philo->last_eat_t = get_time();
		if (++(philo->cnt_eat) == philo->info->arg.must_eat)
		{
			philo->info->stat.n_full++;
			if (philo->info->stat.n_full == philo->info->arg.n_philo)
				philo->info->stat.end++;
		}
	}
	pthread_mutex_unlock(&info->mutex.print);
}

int	take_fork(t_philo *philo)
{
	int	kill_;

	pthread_mutex_lock(&philo->info->mutex.print);
	kill_= philo->info->stat.end;
	pthread_mutex_unlock(&philo->info->mutex.print);
	if (kill_)
		return(ERROR);
	if (philo->idx % 2 == 0)
	{
		pthread_mutex_lock(philo->left);
		philo_print(philo, philo->info, philo->idx, "has taken a fork");
		pthread_mutex_lock(philo->right);
		philo_print(philo, philo->info, philo->idx, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right);
		philo_print(philo, philo->info, philo->idx, "has taken a fork");
		pthread_mutex_lock(philo->left);
		philo_print(philo, philo->info, philo->idx, "has taken a fork");
	}
	return (SUCCESS);
}

int	eating(t_philo *philo, t_arg *arg)
{
	int	kill_;

	pthread_mutex_lock(&philo->info->mutex.print);
	kill_= philo->info->stat.end;
	pthread_mutex_unlock(&philo->info->mutex.print);
	if (kill_)
		return(ERROR);
	philo_print(philo, philo->info, philo->idx, "is eating");
	smart_timer(arg->eat_time);
	return (SUCCESS);
}

int	sleep_thinking(t_philo *philo, t_arg *arg)
{
	int	kill_;

	pthread_mutex_lock(&philo->info->mutex.print);
	kill_= philo->info->stat.end;
	pthread_mutex_unlock(&philo->info->mutex.print);
	
	pthread_mutex_unlock(philo->right);
	pthread_mutex_unlock(philo->left);
	if (kill_)
		return(ERROR);
	philo_print(philo, philo->info, philo->idx, "is sleeping");
	smart_timer(arg->sleep_time);
	philo_print(philo, philo->info, philo->idx, "is thinking");
	// usleep(10);
	return (SUCCESS);
}

void *action(void *param)
{
	t_philo	*philo;
	philo = (t_philo *)param;

	pthread_mutex_lock(&philo->info->mutex.print);
	philo->last_eat_t = get_time();
	pthread_mutex_unlock(&philo->info->mutex.print);
	if (philo->idx % 2 != 0)
		smart_timer(philo->info->arg.eat_time / 2);
	while (!take_fork(philo)
		 && !eating(philo, &philo->info->arg)
		 && !sleep_thinking(philo, &philo->info->arg));
	return (NULL);
}

int init_philo(t_philo **philo, t_info *info, t_arg *arg, pthread_mutex_t *fork)
{
	int	i;

	i = 0;
	(*philo) = malloc(sizeof(t_philo) * arg->n_philo);
	if (*philo == NULL)
		return (ERROR);
	while (i < arg->n_philo)
	{
		(*philo)[i].idx = i;
		(*philo)[i].cnt_eat = 0;
		(*philo)[i].info = info;
		(*philo)[i].left = &fork[i];
		(*philo)[i].right = &fork[(i + 1) % arg->n_philo];
		(*philo)[i].last_eat_t = get_time();
		// philo[i].life_time = 0;
		++i;
	}
	i = -1;
	pthread_mutex_lock(&info->mutex.print);
	while (++i < arg->n_philo)
		if (pthread_create(&(*philo)[i].tid, NULL, action, &(*philo)[i]))
			return (ERROR);
	info->birth_t = get_time();
	pthread_mutex_unlock(&info->mutex.print);
	return (SUCCESS);
}

int init_mutex(t_info *info, pthread_mutex_t **fork)
{
	int	i;

	*fork = malloc(sizeof(pthread_mutex_t) * info->arg.n_philo);
	if (!(*fork))
		return (ERROR);
	i = 0;
	while(i < info->arg.n_philo)
		if (pthread_mutex_init(&(*fork)[i++], PTHREAD_MUTEX_NORMAL) != 0)
			return (ERROR);
	if (pthread_mutex_init(&info->mutex.print, PTHREAD_MUTEX_NORMAL) != 0)
		return (ERROR);
	return (SUCCESS);
}

int init_info(t_philo **philo, t_info *info)
{
	pthread_mutex_t *fork;

	info->stat.end = 0;
	info->stat.n_full = 0;
	if (init_mutex(info, &fork) == ERROR)
		return (ERROR);
	if (init_philo(philo, info, &info->arg, fork) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

//my_atoi memo
/*
	- 음수가 들어왔을 때
	- 문자가 들어왔을 때
	- 아주 큰 숫자가 들어오거나
	return (0) <----------------
*/
int	parse_arg(int argc, char **argv, t_info *info)
{
	if (!(argc == 5 || argc == 6))
		return (ERROR);
	info->arg.n_philo = atoi(argv[1]);
	info->arg.die_time = atoi(argv[2]);
	info->arg.eat_time = atoi(argv[3]);
	info->arg.sleep_time = atoi(argv[4]);
	if (argc == 6)
		info->arg.must_eat = atoi(argv[5]);
	else
		info->arg.must_eat = 0;
	if (info->arg.n_philo <= 0 || info->arg.die_time <= 0 || info->arg.eat_time < 0 || info->arg.sleep_time < 0 || info->arg.must_eat < 0)
		return (ERROR);
	return (SUCCESS);
}

void	mutex_free(t_philo *philo)
{
	int i;

	i = -1;
	while (++i < philo->info->arg.n_philo)
		pthread_mutex_destroy(philo[i].left);
	pthread_mutex_destroy(&philo->info->mutex.print);
}

void	monitor(t_philo *philo)
{
	int	i;
	int	kill_;

	kill_ = 0;
	while (1)
	{
		i = 0;
		while (i < philo->info->arg.n_philo)
		{
			pthread_mutex_lock(&philo->info->mutex.print);
			kill_ = philo->info->stat.end;
			if (get_time() - philo[i].last_eat_t > philo->info->arg.die_time)
			{
				philo->info->stat.end++;
				printf("%ld %d died\n", get_time() - philo->info->birth_t, i + 1);
				pthread_mutex_unlock(&philo->info->mutex.print);
				return ;
			}
			else
				pthread_mutex_unlock(&philo->info->mutex.print);
			if (kill_)
				return ;
			// pthread_mutex_lock(&philo->info->mutex.print);
			// if (philo->info->stat.n_full >= philo->info->arg.must_eat) 
			// {
			// 	philo->info->stat.end++;
			// 	pthread_mutex_unlock(&philo->info->mutex.print);
			// 	return ;
			// }
			// else
			// 	pthread_mutex_unlock(&philo->info->mutex.print);
			++i;
		}
	}
}

int	main(int argc, char **argv)
{
	int		i;
	t_info	info;
	t_philo	*philo;

	if (parse_arg(argc, argv, &info) == ERROR)
		return (ERROR);
	if (init_info(&philo, &info) == SUCCESS)
		monitor(philo);
	i = -1;
	while (++i < philo->info->arg.n_philo)
		pthread_mutex_unlock(philo[i].left);
	i = -1;
	while (++i < philo->info->arg.n_philo)
		pthread_join(philo[i].tid, NULL);
	mutex_free(philo);
	return (0);
}
	// free(philo);
	// free(philo[0].l_fork);
  	// free(info.mutex.fork);