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

void *eat_checker(void *param)
{
	t_philo *philo;

	philo = (t_philo *)param;
	sem_wait(philo->info.sema.eat_checker);			// 1이되면
	kill(-1, SIGKILL);
    sem_close(philo->info.sema.eat_checker);			// 세마포어 종료 및 할당한 자원 해제
	sem_unlink("eat_checker"); 							// 세마포어 객체 해제 제거
	return (NULL);
}

void	philo_print(t_philo *philo, t_info *info, int idx, char *str)
{
	sem_wait(philo->info.sema.print);
	printf("%ld %d %s\n", get_time() - info->birth_t, idx + 1, str);
	if (strstr(str, "is eating"))
	{
		philo->last_eat_t = get_time();
		if (++(philo->eat_cnt) == info->arg.must_eat)
			sem_post(info->sema.eat_checker);
	}
	sem_post(philo->info.sema.print);
}

int take_fork(t_philo *philo)
{
	sem_wait(philo->info.sema.fork);
	philo_print(philo, &philo->info, philo->idx, "has taken a fork");
	sem_wait(philo->info.sema.fork);
	philo_print(philo, &philo->info, philo->idx, "has taken a fork");
	return (SUCCESS);
}

int	eating(t_philo *philo)
{
	philo_print(philo, &philo->info, philo->idx, "is eating");
	smart_timer((*philo).info.arg.eat_time);
	return (SUCCESS);
}

int	sleep_thinking(t_philo *philo)
{
	sem_post(philo->info.sema.fork);
	sem_post(philo->info.sema.fork);
	smart_timer(philo->info.arg.sleep_time);
	return (SUCCESS);
}

void	*monitor(void *param)
{
	t_philo	*const philo  = (t_philo *)param;
	
	while (1)
	{
		sem_wait(philo->info.sema.print);
		if (get_time() > philo->info.arg.die_time + philo->last_eat_t)
		{
			philo_print(philo, &philo->info, philo->idx, "is died");
			// free
			exit(1);
		}
		sem_post(philo->info.sema.print);
	}
	return (NULL);
}

void action(t_philo philo)
{
	pthread_t	tid;

	tid = NULL;
	pthread_create(&tid, NULL, monitor, &philo);
	sem_wait(philo.info.sema.print);
	philo.last_eat_t = get_time();
	sem_post(philo.info.sema.print);
	while (!take_fork(&philo)
			&& !eating(&philo)
			&& !sleep_thinking(&philo));
	exit(1);
}

int init_philo(t_philo *philo, t_info *info, t_arg *arg)
{
	memset(philo, 0, sizeof(t_philo));
	(*philo).last_eat_t = get_time();
	info->sema.fork = sem_open("sem_fork", O_CREAT | O_EXCL, 0644, arg->n_philo);
	if (info->sema.fork == SEM_FAILED)
	{
		sem_unlink("sem_fork");
		info->sema.fork = sem_open("sem_fork", O_CREAT | O_EXCL, 0644, arg->n_philo);
	}
	info->sema.print = sem_open("sem_print", O_CREAT | O_EXCL, 0644, 1);
	if (info->sema.print == SEM_FAILED)
	{
		sem_unlink("sem_print");
		info->sema.print = sem_open("sem_print", O_CREAT | O_EXCL, 0644, 1);
	}
	info->sema.eat_checker = sem_open("eat_checker", O_CREAT | O_EXCL, 0644, 1 - arg->n_philo);
	if (info->sema.eat_checker == SEM_FAILED)
	{
		sem_unlink("eat_checker");
		info->sema.eat_checker = sem_open("eat_checker", O_CREAT | O_EXCL, 0644, 1 - arg->n_philo);
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
	t_philo		philo;
	pthread_t	tid_eatchecker;
	pid_t	pid;
	int		idx;

	if (parse_arg(argc, argv, &philo.info) == ERROR)
		return (ERROR);
	if (init_philo(&philo, &philo.info, &philo.info.arg) == ERROR)
		return (ERROR);
	idx = 0;
	while (idx < philo.info.arg.n_philo)
	{
		pid = fork();
		if (pid == 0)
			action(philo);
		philo.idx = ++idx;
	}

	if (argc == 6)
	{
		pthread_create(&tid_eatchecker, NULL, eat_checker, &philo);
		pthread_detach(tid_eatchecker);
	}
	waitpid(-1, 0, 0);
	kill(-1, SIGKILL);
    sem_close(philo.info.sema.fork);	// 세마포어 종료 및 할당한 자원 해제
    sem_close(philo.info.sema.print);	// 세마포어 종료 및 할당한 자원 해제
	sem_unlink("sem_fork"); 			// 세마포어 객체 해제 제거
	sem_unlink("sem_print"); 			// 세마포어 객체 해제 제거
	return (0);
}
