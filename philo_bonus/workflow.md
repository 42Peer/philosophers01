/*
* 						SUDOCODE
*/

int	forken()
{
	sem_wait();
		print("has taken a fork")
	sem_wait();
		print("has taken a fork")
	eat_cnt++;
	return ()

}

int	sleeping_thinking()
{

	sem_post()
	sem_post()

	print("is sleeping")
	smart_timer()
	print("thinking")
	return();
}

action(t_philo philo,     ) :
{
	sem_open(phi);
	pthread_create(tid, NULL, monitor, param);
    while ()
        routine : forken, eating, sleeping, thinking
    return();
	// exit();
}

int main() -> 시작
{
	포크 n개 파기
	pid_t 	*pid;
	int		i;

    //oflag : 세마 포어 생성시 플래그(O_CREAT, O_EXCL의 조합)
	if (parse_arg(argc, argv, &info) == ERROR)
		return (ERROR);
	if (init_info(&philo, &info) == ERROR)
		return (ERROR);
	파싱_mandatory()

    if (sem = sem_open("test_sem", O_CREAT | O_EXCL, 0644, 3) == SEM_FAILED)
	{
		sem_unlink("test_sem")
		sem = sem_open("test_sem", O_CREAT | O_EXCL, 0644, 3);
	} // value(counter)가 3인 세마포어 생성
	<!--if (sem ==        ) // 세마포어 생성 실패시
	{
		fprintf(stderr, "sem errro\n");
		return (-1);
	}-->

    반복 (i < 사람 수)
    {
        pid = fork() : 자식 생성
        if (자식 일 때)
            action(philo) : 자식 함수 실행 -> return이면 // exit이면
        philo.idx = i;
		i++;
    }

	var status;
    waitpid(-1, &status, 0); 자식 프로세스 기다리기
	if (WIFSIGNALED(status) : DIE일 때 || WEXITSTATUS(status) : MUST EAT 채워서 끝났는지 체크하기)
	{
			모든 프로세스 KILL
			if (died)
				n is died 출력 후 종료
			else if (MUST EAT 다채우면)
				eating으로 끝내게 하기
	}
	while (나머지 철학자들)
	{
		waitpid 로 회수
	}
    free : 모두 자유롭게
    ret = sem_close(sem); // 세마포어 종료 및 할당한 자원 해제
	printf("sem_close: %d\n", ret);
	ret = sem_unlink("test_sem"); // 세마포어 객체 해제
	printf("sem_unlink: %d\n", ret);
    return (0);
}

고아 : 자식보다 부모가 먼저 끝남
좀비 : 자식 끝났는데 수거를 안 함.


/*
sem_t *sem_open(const char *name, int oflag, [mode], [value])

name : 생성 혹은 접근하고자하는 세마포어 이름
oflag : 세마 포어 생성시 플래그(O_CREAT, O_EXCL의 조합)
mode_t modes : 플래그를 O_CREAT로 설정하면, mode 인자를 받을 수 있다. <sys/stat.h> 헤더를 이용하면 아래 상수들을 활용할 수 있다.
S_IRWXR :그룹 접근
S_IRWXO: 타인 접근
S_IRWXU: 개인 접근
unsigined int value : 세마포어 초기 값으로 0보다 큰 양수이어야 한다. unlock된 세마포어의 수를 의미한다. 이 값은 SEM_VALUE_MAX를 초과할 수 없다.
*/me : 생성 혹은 접근하고자하는 세마포어 이름
oflag : 세마 포어 생성시 플래그(O_CREAT, O_EXCL의 조합)
mode_t modes : 플래그를 O_CREAT로 설정하면, mode 인자를 받을 수 있다. <sys/stat.h> 헤더를 이용하면 아래 상수들을 활용할 수 있다.
S_IRWXR :그룹 접근
S_IRWXO: 타인 접근
S_IRWXU: 개인 접근
unsigined int value : 세마포어 초기 값으로 0보다 큰 양수이어야 한다. unlock된 세마포어의 수를 의미한다. 이 값은 SEM_VALUE_MAX를 초과할 수 없다.
*/