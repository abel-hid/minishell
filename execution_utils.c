/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 15:22:40 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/06 13:12:24 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_overflow(unsigned long long res, int n)
{
	if (n == 1)
	{
		if (res >= 9223372036854775808ULL)
		{
			g_exit_st.status = 255;
			return (g_exit_st.status);
		}
	}
	if (n == -1)
	{
		if (res == 9223372036854775808ULL)
		{
			g_exit_st.status = 0;
			return (g_exit_st.status);
		}
		if (res >= 9223372036854775809ULL)
		{
			g_exit_st.status = 255;
			return (g_exit_st.status);
		}
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		n;
	long	result;
	int		overflow;

	i = 0;
	n = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			n = -1;
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + str[i] - 48;
		overflow = check_overflow(result, n);
		if (overflow != 1)
			return (overflow);
		i++;
	}
	return ((result * n));
}

t_env	*ft_lst_new( char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	return (new);
}

int	ft_isaplpha(char c)
{
	if ((c >= 'a' && c <= 'z' ) || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}
