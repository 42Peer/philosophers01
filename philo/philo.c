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

	(*info).thread = malloc(sizeof(pthread_t) * (*info).arg.philo_n);
	if ((*info).thread == NULL)
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

int	main(int argc, char *argv[])
{
	t_info	info;

	if (!(argc == 5 || argc == 6))
		return (ft_error(&info));
	set_arg(argc, argv, &info);
	if (init_info == ERROR)
		return (ft_error(&info));
	return (0);
}