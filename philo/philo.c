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
	pthread_mutex_init((*info).prt_mutex, NULL);
	return (SUCCESS);
}

void	*philo_action(void *param)
{
	t_philo *philo;

	philo = (t_philo *)param;
	philo_fork(philo);
	philo_eat(philo);
	philo_sleep(philo);
	philo_think(philo);
	return (NULL);
}

t_philo	*init_philo(t_info info)
{
	int		n;
	int		i;
	t_philo	*philo;

	n = info.arg.philo_n;
	i = 0;
	philo = malloc(info.arg.philo_n * sizeof(t_philo));
	while (i < n)
	{
		philo[i].idx = i;
		philo[i].info = info;
		++i;
	}
	i = 0;
	while (i < n)
	{
		pthread_create(philo[i].tid, NULL, &philo_action, &philo[i]);
		++i;
	}
}


int	main(int argc, char *argv[])
{
	t_info	info;
	t_philo	*philo;

	if (!(argc == 5 || argc == 6))
		return (ft_error(&info));
	set_arg(argc, argv, &info);
	init_info(&info);
	init_philo(info);
	return (0);
}

//detach 되었을 때, mutex안됨
//다 죽은 다음에 해야함
//근데 다시 확인은 해봐야함