/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 13:15:41 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/07 14:10:19 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parsing_unset(char *str)
{
	int	i;

	i = 0;
	if (*str <= '9' && *str >= '0')
	{
		ft_error("minishell: unset: `", str, "': not a valid identifier\n");
		g_exit_st.status = 1;
		return (1);
	}
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			ft_error("minishell: unset: `", str, "': not a valid identifier\n");
			g_exit_st.status = 1;
			return (1);
		}
		i++;
	}
	return (0);
}

void	check_for_path(t_command *cmd)
{
	if (cmd->args[1] && !ft_strcmp(cmd->args[1], "PATH") && !g_exit_st.is_unset)
	{
		free(g_exit_st.path);
		g_exit_st.is_unset = 1;
	}
}

void	unset_thedummy(t_env **g_env, t_command *cmd, int i, t_env	*prev)
{
	t_env	*tmp;

	tmp = *g_env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, cmd->args[i]) == 0)
		{
			if (prev == NULL)
				*g_env = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			break ;
		}
		g_exit_st.status = 0;
		prev = tmp;
		tmp = tmp->next;
	}
}

void	ft_unset(t_command *cmd, t_env **g_env)
{
	t_env	*prev;
	int		i;

	i = 1;
	prev = NULL;
	check_for_path(cmd);
	while (cmd->args[i])
	{
		if (parsing_unset(cmd->args[i]) == 1)
			return ;
		unset_thedummy(g_env, cmd, i, prev);
		i++;
	}
}
