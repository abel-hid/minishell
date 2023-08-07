/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 15:32:12 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/07 14:10:34 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_key2(char *key, char	*arg)
{
	if (check_key(key, arg) == 45)
		return ;
}

int	check_if_existant(char *key, t_env **g_env)
{
	t_env	*tmp;

	tmp = *g_env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	export_norm(char *key, char *value, t_env **g_env)
{
	if (value == NULL)
	{
		reset_this(key);
		if (_check(key, g_env) == 0)
		{
			if (check_if_existant(key, g_env) == 0)
				lstadd_back(g_env, new_env(key, NULL));
		}
		else
			free(key);
	}
}

void	reset_this(char *key)
{
	if (ft_strcmp(key, "PATH") == 0 && !g_exit_st.is_unset)
	{
		g_exit_st.is_unset = 81;
		free(g_exit_st.path);
		g_exit_st.path 
			= ft_strdup("/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
	}
}
