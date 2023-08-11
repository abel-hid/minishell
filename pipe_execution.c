/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 15:37:34 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/10 00:33:08 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_on_pipes(t_command *cmd, t_env **g_env, char **envp)
{
	int	fd[2];
	int	old;
	int	a;

	a = 0;
	fd[0] = -1;
	fd[1] = -1;
	while (cmd)
	{
		if (cmd->args[0])
			a = checkfor_builtins(cmd);
		old = fd[0];
		protection(fd);
		cmd->pid = fork();
		if (cmd->pid == 0)
		{
			handle_child_process(cmd, fd, old);
			execute_all(cmd, g_env, envp, a);
		}
		else if (cmd->pid > 0)
			close_old(old, fd);
		cmd = cmd->next;
	}
	close(fd[1]);
	close(fd[0]);
}

void	srfak_lbn(t_command *cmd, t_env **g_env, char **envp)
{
	handle_one_command(cmd);
	execute_bin(cmd, envp, g_env);
}

void	norminette_help(t_command	*cmd, t_env	**g_env, char	**envp)
{
	cmd->pid = fork();
	if (cmd->pid == 0)
		srfak_lbn(cmd, g_env, envp);
	if (cmd->pid > 0)
		wait_for_all(cmd);
}

int	execute_the_shot(t_command	*cmd, t_env **g_env, char **envp)
{
	int			a;
	t_command	*tmp;

	tmp = cmd;
	a = 0;
	if (!cmd->args[0] &&!cmd->next)
		return (0);
	if (!cmd->next)
	{
		a = checkfor_builtins(cmd);
		if (a == 1)
			execute_built_ins(cmd, g_env);
		if (a == 0)
			norminette_help(cmd, g_env, envp);
		return (0);
	}
	execute_on_pipes(cmd, g_env, envp);
	wait_for_all(tmp);
	return (0);
}
