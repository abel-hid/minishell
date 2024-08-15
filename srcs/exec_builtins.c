/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 19:56:33 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/06 19:56:37 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dup_andclose(t_command *cmd)
{
	if (cmd->fd.fd_in != 0)
	{
		if (cmd->fd.fd_in == -1)
		{
			g_exit_st.status = 1;
			return (1);
		}
		g_exit_st.in = dup(0);
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
		g_exit_st.out = dup(1);
		dup2(cmd->fd.fd_out, 1);
		close(cmd->fd.fd_out);
	}
	return (0);
}

void	close_everything(t_command	*cmd)
{
	if (cmd->fd.fd_in != 0)
	{
		dup2(g_exit_st.in, 0);
		close(g_exit_st.in);
	}
	if (cmd->fd.fd_out != 1)
	{
		dup2(g_exit_st.out, 1);
		close(g_exit_st.out);
	}
}

int	execute_built_ins(t_command *cmd, t_env **envp)
{
	if (dup_andclose(cmd))
		return (1);
	check_and_execute_builtins(cmd, envp);
	close_everything(cmd);
	return (1);
}
