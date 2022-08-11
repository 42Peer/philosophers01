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


void	philo_print(t_info *info, int idx, char *status)
{
	pthread_mutex_lock(&info->mutex.print);
	if (pilo->info->stat.end != 0)
		;
	else
		printf("%ld %d %s\n", get_time() - info->birth_time, idx + 1, status);
	pthread_mutex_unlock(&info->mutex.print);
}

int	take_fork(t_philo *philo, t_mutex *mutex)
{
	if (philo->idx % 2 == 0)
	{
		pthread_mutex_lock(philo->left);
//		if (flag_checker)
//			return (pthread_mutex_unlock() != 0);
//		else
		philo_print(philo->info, philo->idx, "has taken a fork");
	else
	{
//		pthread_mutex_lock(philo->right);
//		if (flag_checker)
//			return (pthread_mutex_unlock() != 0);
//		else
		philo_print(philo->info, philo->idx, "has taken a fork");
//		philo_print(philo->info, philo->idx, "has taken a fork");
	}
	return (SUCCESS);
}

void *action(void *param)
{
	t_info	*info;

	philo = (t_philo *)param;

	while (take_fork(philo, info->mutex, info->philo[i]) && 
			eating(info) && sleeping(info) && thinking(info))
	{
		info->philo[]->idx++;
	}
}

int init_philo(, t_philo *philo, t_info *info, t_arg *arg, pthread_mutex_t *fork)
{
	int	i;

	i = 0;
	philo = malloc(sizeof(t_philo) * arg->n_philo);
	if (info->philo == NULL)
		return (ERROR);
	while (i < arg->n_philo)
	{
		philo[i].idx = i;
		philo[i].cnt_eat = 0;
		philo[i].info = info;
		philo[i].left = &fork[i];
		philo[i].right = &fork[(i + 1) % arg->n_philo];
		// philo[i].life_time = 0;
		++i;
	}
	i = 0;
	info.i = 0;
	pthread_mutex_lock(&info->mutex.sub);
	while (i < arg->n_philo)
		if (pthread_create(&philo[i++].tid, NULL, action, &philo[i]))
			return (ERROR);
		info->birth_t = get_time();
	pthread_mutex_unlock(&info->mutex.sub);
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
	if (pthread_mutex_init(&info->mutex.sub, PTHREAD_MUTEX_NORMAL) != 0)
		return (ERROR);
	return (SUCCESS);
}

int init_info(t_philo *philo, t_info *info)
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
}

int main(int argc, char **argv)
{
	t_philo	*philo;
	t_info	info;

	if (parse_arg(argc, argv, &info) == ERROR)
		return (ERROR);
	if (init_info(&philo, &info) == SUCCESS)
		monitor();
	free(info.philo);
	free(info.mutex.fork);
	return (0);
}
