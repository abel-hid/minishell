/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 15:03:50 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/06 13:12:10 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_one_command(t_command *cmd)
{
	if (cmd->fd.fd_out != 1)
	{
		if (cmd->fd.fd_out == -1)
		{
			g_exit_st.status = 1;
			exit(1);
		}
		dup2(cmd->fd.fd_out, 1);
		close(cmd->fd.fd_out);
	}
	if (cmd->fd.fd_in != 0)
	{
		if (cmd->fd.fd_in == -1)
		{
			g_exit_st.status = 1;
			exit(1);
		}
		dup2(cmd->fd.fd_in, 0);
		close(cmd->fd.fd_in);
	}
}

void	execute_all(t_command *cmd, t_env **g_env, char **envp, int a)
{
	if (a == 1)
		execute_built_ins(cmd, g_env);
	if (a == 0)
		execute_bin(cmd, envp, g_env);
	exit(g_exit_st.status);
}
