/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 14:51:22 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/07 14:10:45 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lstsize(t_command *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

int	checkfor_builtins(t_command *cmd)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (1);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (1);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (1);
	return (0);
}

void	check_and_execute_builtins(t_command *cmd, t_env **envp)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		ft_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0 && lstsize(cmd) == 1)
		ft_cd(cmd, envp);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(cmd->args[0], "export") == 0)
	{
		if (ft_export(cmd, envp) == 1)
			g_exit_st.status = 1;
		else
			g_exit_st.status = 0;
	}
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		ft_env(envp);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		ft_unset(cmd, envp);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		ft_exit(cmd);
}
