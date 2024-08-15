/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 14:46:14 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/07 12:02:05 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_path(t_env **g_env, char *path)
{
	t_env	*tmp;
	char	cwd[1024];

	tmp = *g_env;
	while (tmp)
	{
		getcwd(cwd, sizeof(cwd));
		if (path)
			free(path);
		path = ft_strdup(cwd);
		if (ft_strcmp(tmp->key, "PWD") == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(path);
		}
		tmp = tmp->next;
	}
	free(path);
}

void	change_directory(char *path, char *args)
{
	if (chdir(path) == -1 && g_exit_st.status != 1)
	{
		ft_error("minishell: cd: ", args, ": No such file or directory\n");
		g_exit_st.status = 1;
		return ;
	}
	else
		g_exit_st.status = 0;
}
