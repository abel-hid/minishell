/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 15:29:27 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/06 13:12:47 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_key_value(char *arg, t_env **g_env)
{
	int		j;
	t_env	*tmp;
	char	*key;
	char	*value;

	tmp = *g_env;
	j = 0;
	check_args(arg);
	while (arg[j] && (arg[j] != '=' && (arg[j] != '+')))
		j++;
	key = ft_substr(arg, 0, j);
	check_key2(key, arg);
	value = NULL;
	if (arg[j] == '+')
	{
		value = ft_substr(arg, j + 2, ft_strlen(arg) - j - 2);
		update_or_add_env_var(g_env, key, value);
	}
	else if (arg[j] == '=')
	{
		value = ft_substr(arg, j + 1, ft_strlen(arg) - j - 1);
		add_env_var(g_env, key, value);
	}
	export_norm(key, value, g_env);
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

void	print_export(t_env **env_list)
{
	t_env	*tmp;

	tmp = *env_list;
	while (tmp)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(tmp->key, 1);
		if (tmp->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(tmp->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
		tmp = tmp->next;
	}
}

void	ft_export(t_command *cmd, t_env **p_env)
{
	int	i;

	i = 1;
	while (cmd->args[i] != NULL)
	{
		if (cmd->args[i] != NULL && !check_for_equal(cmd->args[i]))
		{
			if (check_key(cmd->args[i], cmd->args[i]) == 45)
			{
				g_exit_st.status = 1;
				return ;
			}
		}
		create_key_value(cmd->args[i], p_env);
		i++;
	}
	if (cmd->args[1] == NULL)
		print_export(p_env);
}
