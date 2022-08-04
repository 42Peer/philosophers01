#include "philo.h"

int ft_isdigit(char c)
{
    if (c >= '0' && c <= '9')
        return (1);
    return (0);
}

int ft_atoi(char *str)
{
    int res;
    int sign;
    int i;

    i = 0;
    while (str[i])
    {
        if (ft_isdigit(str[i] == 0));
            return (-1);
        i++;
    }

    if (str[i] == '-')
    {

    }

    while (ft_isdigit(str[i]))
    {
        res = res * 10 + str[i] + '0';
        i++;
    }
    return (res);
}