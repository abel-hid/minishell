/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_bin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 15:04:52 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/06 13:12:13 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**path_splitted(t_env **g_env)
{
	int		i;
	char	**paths;
	t_env	*tmp;

	tmp = *g_env;
	i = 0;
	if (!g_exit_st.path)
	{
		if (!*g_env && (!g_exit_st.is_unset))
			g_exit_st.path 
				= ft_strdup("/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
		else
			g_exit_st.path = ft_strdup("");
	}
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "PATH") == 0 && tmp->value != NULL)
			g_exit_st.path = ft_strdup(tmp->value);
		tmp = tmp->next;
	}
	paths = ft_split(g_exit_st.path, ':');
	return (paths);
}

void	norm_help(t_command *cmd, int j, int existance, char c)
{
	if (cmd->args[0][j] == c)
	{
		if (cmd->args[0][j + 1] == '/' || cmd->args[0][j + 1] == '.')
		{
			if (existance == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd->args[0], 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				exit(127);
			}
		}
	}
}

void	print_err(char *str)
{
	ft_error("minishell: ", str, ": Permission denied\n");
	g_exit_st.status = 126;
	exit(126);
}

void	_manipulate_files(int permission, int existance, t_command *cmd)
{
	int	j;

	j = 0;
	while (cmd->args[0][j])
	{
		if (!existance && permission == -1)
			print_err(cmd->args[0]);
		else
		{
			if (cmd->args[0][0] == '/')
			{
				if (permission == -1)
				{
					ft_error("minishell: ", cmd->args[0], 
						": No such file or directory\n");
					g_exit_st.status = 127;
					exit(127);
				}
			}
		}
		norm_help(cmd, j, existance, '/');
		norm_help(cmd, j, existance, '.');
		j++;
	}
}

int	check_filepermission_ndexistance(t_command *cmd)
{
	int	file_existance;
	int	file_permission;

	file_existance = access(cmd->args[0], F_OK);
	file_permission = access(cmd->args[0], X_OK);
	if ((cmd->args[0][0] == '.' || cmd->args[0][0] == '/'))
	{
		_manipulate_files(file_permission, file_existance, cmd);
		return (1);
	}
	return (0);
}
