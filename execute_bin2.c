/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_bin2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 15:16:10 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/06 13:12:19 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exec_ve(t_command *cmd, char *lwa, char	**envi)
{
	if (execve(lwa, cmd->args, envi) == -1)
	{
		g_exit_st.status = 127;
		if (!ft_strcmp(lwa, "") && g_exit_st.is_unset == 1)
		{
			ft_error("minishell: ", cmd->args[0],
				": No such file or directory\n");
			exit(127);
		}
		if (check_filepermission_ndexistance(cmd) 
			&& access(cmd->args[0], F_OK | X_OK) == 0)
		{
			ft_error("minishell: ", cmd->args[0], ": is a directory\n");
			exit(126);
		}
		lwa = cmd->args[0];
		ft_error("minishell: ", cmd->args[0], ": command not found\n");
		exit(g_exit_st.status);
	}
}

void	execute_bin(t_command	*cmd, char	**envi, t_env	**g_env)
{
	char	*lwa;
	int		i;
	char	**paths;
	char	*tmp;

	i = 0;
	lwa = ft_strdup("");
	paths = path_splitted(g_env);
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		tmp = ft_strjoin(tmp, cmd->args[0]);
		if (access(tmp, F_OK | X_OK) == 0)
			lwa = ft_strdup(tmp);
		if (check_filepermission_ndexistance(cmd))
			lwa = ft_strdup(cmd->args[0]);
		i++;
	}
	ft_exec_ve(cmd, lwa, envi);
}
