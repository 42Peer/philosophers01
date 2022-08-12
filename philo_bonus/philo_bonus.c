#include "philo_bonus.h"

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

int take_fork(t_philo philo)
{
	sem_wait(philo.info.sema.fork);
	philo_print(philo, philo.info, philo.idx, "has taken a fork");
	sem_wait(philo.info.sema.fork);
	philo_print(philo, philo.info, philo.idx, "has taken a fork");
}

int	eating(t_philo *philo)
{
	int	kill_;

	kill_= (*philo).info.stat.end;
	if (kill_)
		return(kill(););
	philo_print(philo, philo->info, philo->idx, "is eating");
	smart_timer((*philo).info.arg.eat_time);
	return (SUCCESS);
}

void	sleep_thinking(t_philo *philo)
{
	sem_post(philo->info.sema.fork);
	sem_post(philo->info.sema.fork);
	smart_timer(philo->info.arg.sleep_time);
}

void action(t_philo philo)
{
	//pthread_create()
	sem_wait(philo.info.sema.print);
	philo.last_eat_t = get_time();
	sem_post(philo.info.sema.print);
	while (!take_fork(&philo)
			&& !eating(&philo)
			&& !sleep_thinking(&philo));
	exit(1);
}

int	check_eat(sem_t cnt)
{

}

int init_philo(t_philo *philo, t_info *info, t_arg *arg)
{
	// memset_pattern4
	memset(philo, 0, sizeof(t_philo));
	(*philo).last_eat_t = get_time();
	(*philo).info.sema.fork = sem_open("sem_fork", O_CREAT | O_EXCL, 0644, (*philo).info.arg.n_philo);
	if ((*philo).info.sema.fork == SEM_FAILED)
	{
		sem_unlink("sem_fork");
		(*philo).info.sema.fork = sem_open("sem_fork", O_CREAT | O_EXCL, 0644, (*philo).info.arg.n_philo);
	}
	(*philo).info.sema.print = sem_open("sem_print", O_CREAT | O_EXCL, 0644, 1);
	if ((*philo).info.sema.print == SEM_FAILED)
	{
		sem_unlink("sem_print");
		(*philo).info.sema.print = sem_open("sem_print", O_CREAT | O_EXCL, 0644, 1);
	}
	(*philo).info.sema.print = sem_open("sem_print", O_CREAT | O_EXCL, 0644, 0);
	if ((*philo).info.sema.print == SEM_FAILED)
	{
		sem_unlink("sem_print");
		(*philo).info.sema.print = sem_open("sem_print", O_CREAT | O_EXCL, 0644, 0);
	}
	return (SUCCESS);
}

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

int	main(int argc, char **argv)
{
	t_philo	philo;
	pid_t	pid;
	int		idx;

	if (parse_arg(argc, argv, &philo.info) == ERROR)
		return (ERROR);
	if (init_philo(&philo, &philo.info, &philo.info.arg) == ERROR)
		return (ERROR);

	while (idx < philo.info.arg.n_philo)
	{
		pid = fork();
		if (pid == 0)
			action(philo);
		philo.idx = ++idx;
	}
	check_eat();
	waitpid(-1, &philo.info.stat, 0);

/*
	WIFSIGNALED(status);
	WEXITSTATUS(status);
*/

	return (0);
}
