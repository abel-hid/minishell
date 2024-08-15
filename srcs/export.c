/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 15:29:27 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/11 02:17:32 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_for_second(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] != '=' && arg[i] != '+')
		i++;
	return (i);
}

int	hah(char c, char *arg)
{
	if (c != '=')
	{
		ft_error("minishell: export: `", arg, "': not a valid identifier\n");
		return (1);
	}
	return (0);
}

int	create_key_value(char *arg, t_env **g_env)
{
	int		j;
	char	*key;
	char	*value;

	if (check_args(arg))
		return (1);
	j = check_for_second(arg);
	key = ft_substr(arg, 0, j);
	if (check_key(key, arg) == 45)
		return (free(key), 1);
	value = NULL;
	if (arg[j] == '+')
	{
		if (hah(arg[j + 1], arg))
			return (free(key), 1);
		value = ft_substr(arg, j + 2, ft_strlen(arg) - j - 2);
		update_or_add_env_var(g_env, key, value);
	}
	else if (arg[j] == '=')
	{
		value = ft_substr(arg, j + 1, ft_strlen(arg) - j - 1);
		add_env_var(g_env, key, value);
	}
	return (export_norm(key, value, g_env), 0);
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

int	ft_export(t_command *cmd, t_env **p_env)
{
	int	i;

	i = 1;
	while (cmd->args[i] != NULL)
	{
		if (cmd->args[i] != NULL && !check_for_equal(cmd->args[i]))
		{
			if (check_key(cmd->args[i], cmd->args[i]) == 45)
			{
				i++;
				continue ;
			}
		}
		create_key_value(cmd->args[i], p_env);
		i++;
	}
	if (cmd->args[1] == NULL)
		print_export(p_env);
	return (0);
}
