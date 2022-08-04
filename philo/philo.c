#include "philo.h"

int set_arg(char **argv, t_info *info)
{
	(*info).arg.philo_n = ft_atoi(av[1]);
	if ((*info).arg.philo_n < 0)
		return (ERROR);
	(*info).arg.life_t = ft_atoi(av[2]);
	if ((*info).arg.life_t < 0)
		return (ERROR);
	(*info).arg.eat_t = ft_atoi(av[3]);
	if ((*info).arg.eat_t < 0)
		return (ERROR);
	(*info).arg.sleep_t = ft_atoi(av[4]);
	if ((*info).arg.sleep_t < 0)
		return (ERROR);
	if (argc == 6)
	{
		(*info).arg.eat_cnt = ft_atoi(av[5]);
		if ((*info).arg.eat_cnt < 0)
			return (ERROR);
	}
	return (SUCCESS);
}

int	main(int argc, char *argv[])
{
	t_info info;

	if (!(argc == 5 || argc == 6))
		ft_error();

	set_arg(argv, &info);

	info.thread = malloc(sizeof(pthread_t) * )
	if ((*info).thread ==)
	

	return (0);
}
