/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 14:50:18 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/07 14:05:05 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_the_path(char *path, char	*home)
{
	if (!home || !path)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		free(path);
		return ;
	}
	ft_putstr_fd(path, 1);
	ft_putstr_fd("\n", 1);
}

void	ft_cd(t_command *cmd, t_env **g_env)
{
	char	*home;
	char	*path;

	home = getenv("HOME");
	path = getenv("PWD");
	if (cmd->args[1] == NULL)
	{
		if (!home)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return ;
		}
		path = ft_strdup(home);
	}
	else
		path = ft_strdup(cmd->args[1]);
	change_directory(path, cmd->args[1]);
	update_path(g_env, path);
}
