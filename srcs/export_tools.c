/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 15:28:01 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/11 02:37:53 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int empty_key(char *str)
{
	if (ft_strcmp(str, "") == 0 || is_space(str[0]))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(str, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		g_exit_st.status = 1;
		return (45);
	}
	return (0);
}
int	check_key(char *str, char *arg)
{
	if (empty_key(str) == 45)
		return (45);
	if (*str >= '0' && *str <= '9')
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(str, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		g_exit_st.status = 1;
		return (45);
	}
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
		{
			ft_error("minishell: export: `", arg, "': not a valid identifier\n");
			g_exit_st.status = 1;
			return (45);
		}
		str++;
	}
	return (0);
}

int	_check(char *key, t_env **g_env)
{
	t_env	*tmp;

	tmp = *g_env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	check_args(char *arg)
{
	if (*arg == '=' || *arg == '+' || *arg == '-')
	{
		ft_error("minishell: export: `", arg, "': not a valid identifier\n");
		g_exit_st.status = 1;
		return (1);
	}
	return (0);
}

int	check_for_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}
