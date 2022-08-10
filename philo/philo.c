#include "philo.h"

void *action(void *param)
{
	t_info	*info;

	info = (t_info *)param;

}

int init_mutex(t_info *info)
{
	int	i;

	info->mutex.fork = malloc(sizeof(pthread_mutex_t) * info->arg.n_philo);
	if (info->mutex.fork == NULL)
		return (ERROR);
	i = 0;
	while(i < info->arg.n_philo)
		if (pthread_mutex_init(&info->mutex.fork[i++], PTHREAD_MUTEX_NORMAL) != 0)
			return (ERROR);
	if (pthread_mutex_init(&info->mutex.print, PTHREAD_MUTEX_NORMAL) != 0)
		return (ERROR);
	if (pthread_mutex_init(&info->mutex.sub, PTHREAD_MUTEX_NORMAL) != 0)
		return (ERROR);
	return (SUCCESS);
}

int init_philo(t_info *info, t_philo *philo, t_arg *arg)
{
	int	i;

	i = 0;
	info->philo = malloc(sizeof(t_philo) * arg->n_philo);
	if (info->philo == NULL)
		return (ERROR);
	while (i < arg->n_philo)
	{
		philo[i].idx = i;
		philo[i].cnt_eat = 0;
		// philo[i].life_time = 0;
		++i;
	}
	i = 0;
	pthread_mutex_lock(&info->mutex.sub);
	while (i < arg->n_philo)
		if (pthread_create(&philo[i++].tid, NULL, action, info) != 0)
			return (ERROR);
	pthread_mutex_unlock(&info->mutex.sub);
	
}

int init_info(t_info *info)
{
	info->stat.end = 0;
	info->stat.n_full = 0;
	if (init_mutex(info) == ERROR)
		return (ERROR);
	if (init_philo(info, info->philo, &info->arg) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

int	parse_arg(int argc, char **argv, t_info *info)
{
	if (!(argc == 5 || argc == 6))
		return (ERROR);
	//my_atoi memo
	/*
		- 음수가 들어왔을 때
		- 문자가 들어왔을 때
		- 아주 큰 숫자가 들어오거나
		return (ERROR)
	*/
	info->arg.n_philo = atoi(argv[1]);
	info->arg.t2die = atoi(argv[2]);
	info->arg.t2eat = atoi(argv[3]);
	info->arg.t2sleep = atoi(argv[4]);
	if (argc == 6)
		info->arg.must_eat = atoi(argv[5]);
	else
		info->arg.must_eat = 0;
	if (info->arg.n_philo < 0 || info->arg.t2die < 0 || info->arg.t2eat < 0 || info->arg.t2sleep < 0 || info->arg.must_eat < 0)
		return (ERROR);
}

int main(int argc, char **argv)
{
	t_info	info;

	if (parse_arg(argc, argv, &info) == ERROR)
		return (ERROR);
	if (init_info(&info) == SUCCESS)
		monitor();
	free(info.philo);
	free(info.mutex.fork);
	return (0);
}
