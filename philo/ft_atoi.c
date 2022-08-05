/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 15:15:43 by jujeon            #+#    #+#             */
/*   Updated: 2022/08/05 10:51:49 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void	check_outofint(long long num)
// {
// 	if (num < -2147483648 || num > 2147483647)
// 		get_error(1);
// }

// void	check_noint(char *str)
// {
// 	char	*temp;

// 	while (*str == 32 || (*str >= 9 && *str <= 13))
// 		str++;
// 	if (*str == '-' || *str == '+')
// 		str++;
// 	// temp = (char *)str;
// 	// while (temp[0] == '0' && temp[0] != '\0')
// 	// 	temp++;
// 	// if (ft_strlen(temp) > 10)
// 	// 	get_error(WRONGARG);
// 	while (*str)
// 	{
// 		if (*str < '0' || *str > '9')
// 			return (-1);
// 		str++;
// 	}
// }

long long	ft_atoi(const	char	*str)
{
	long long	minus;
	long long	ret;

	ret = 0;
	minus = 1;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			minus = -1;
		str++;
	}
	if (*str == '-' || *str == '+')
		return (-1);
	while (*str)
	{
		if ('0' <= *str && *str <= '9')
		{
			ret = ret * 10 + *str - '0';
			if ((ret * minus > INT_MAX) || ret * minus < INT_MIN)
				return (-1);
		}
		else
			break ;
		str++;
	}
	return (ret * minus);
}
