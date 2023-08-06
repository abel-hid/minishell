/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_oradd_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 19:58:21 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/06 13:08:12 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_or_add_env_var(t_env **p_env, char *key, char *value)
{
	t_env	*tmp;

	tmp = *p_env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (value)
			{
				free(tmp->value);
				tmp->value = ft_strjoin(tmp->value, value);
				free(value);
				free(key);
			}
			break ;
		}
		tmp = tmp->next;
	}
	if (tmp == NULL)
	{
		tmp = new_env(key, value);
		lstadd_back(p_env, tmp);
	}
}

void	add_env_var(t_env **p_env, char *key, char *value)
{
	t_env	*tmp;

	tmp = *p_env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (value)
			{
				free(tmp->value);
				tmp->value = ft_strdup(value);
				free(value);
				free(key);
			}
			break ;
		}
		tmp = tmp->next;
	}
	if (tmp == NULL)
	{
		tmp = new_env(key, value);
		lstadd_back(p_env, tmp);
	}
}
