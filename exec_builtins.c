/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 15:01:21 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/06 13:11:52 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dup_andclose(int in, int out, t_command *cmd)
{
	if (cmd->fd.fd_in != 0)
	{
		if (cmd->fd.fd_in == -1)
		{
			g_exit_st.status = 1;
			return (1);
		}
		in = dup(0);
		dup2(cmd->fd.fd_in, 0);
		close(cmd->fd.fd_in);
	}
	if (cmd->fd.fd_out != 1)
	{
		if (cmd->fd.fd_in == -1)
		{
			g_exit_st.status = 1;
			return (1);
		}
		out = dup(1);
		dup2(cmd->fd.fd_out, 1);
		close(cmd->fd.fd_out);
	}
	return (0);
}

void	close_everything(int in, int out, t_command	*cmd)
{
	if (cmd->fd.fd_in != 0)
	{
		dup2(in, 0);
		close(in);
	}
	if (cmd->fd.fd_out != 1)
	{
		dup2(out, 1);
		close(out);
	}
}

int	execute_built_ins(t_command *cmd, t_env **envp)
{
	int	in;
	int	out;

	in = 0;
	out = 0;
	if (dup_andclose(in, out, cmd) == 1)
		return (1);
	check_and_execute_builtins(cmd, envp);
	close_everything(in, out, cmd);
	return (1);
}
