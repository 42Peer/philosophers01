



if (저녁 시간)
    한솥 제외
    if (비가 안오면)
        배달 음식(피자, 치킨, )
    else
		{
			저녁메뉴추천좀
			광수육회
			주먹고기
			
		}



flag_checker
{
    int check;
    
    sub_mutex >> lock
    if (!end_f)     => 정상케이스
        check = 1
    else            => 에러케이스
        check = -1
    sub_mutex >> unlock
    return (check);
}


int take_fork(philo, )
{
	if (philo->idx & 2 == 0)
	{
		왼손 mutex lock 
        if (flackchecker == 정상) 
			print();
        else (flackchecker == 에러)
            return (왼손 mutext unlock);

		오른손 mutex lock	
        if (flackchecker == 정상) 
			print();
        else (flackchecker == 에러)
            return (오른손 mutex unclock);
	}
	else
	{
		왼손 mutex lock 
		if (flackchecker == 정상) 
			print();
   		else (flackchecker == 에러)
        	return (왼손 mutext unlock);
		오른손 mutex lock
        if (flackchecker == 정상) 
			print();
        else (flackchecker == 에러)
            return (오른손 mutex unclock);
	}
}

philo_print(t_info *info, idx, char* status)
{
    prt_mutex >> lock   is eating...end_flag 이전
	sub_mutex >> lock
	if (end_flag)
	{
		sub_mutex >> unlock
		현재 상태 출력 <- char *인자로 받기
	else
		sub_mutex >> unlock
    prt_mutex >> unlock
}

action(void *param)
{
	t_info *info;

	philo = (t_pihlo *)param;
	while (포크집기() && 먹기() && 자기() && 생각하기())
	{
		필로의 indx++;
	}
}