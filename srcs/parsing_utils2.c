/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 21:53:53 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/04 21:58:17 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**realloc_args(char **args, int count)
{
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	return (args);
}

t_fd	ft_fd(int fd_in, int fd_out)
{
	t_fd	fd;

	fd.fd_in = fd_in;
	fd.fd_out = fd_out;
	return (fd);
}

int	handel_append(char *str_next, int fd, t_env **g_env)
{
	char	*str;

	str = ft_strdup(str_next);
	str = ft_expand(str, g_env);
	if (ft_strcmp(str, ""))
	{
		if (ambiguous_redirect(str, g_env, str_next) == 1)
			return (-1);
		str = del_quote(str, '\'', '\"');
		fd = open(str, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
		{
			error(str);
			return (free(str), -1);
		}
	}
	else
	{
		error_ambiguous(str_next);
		return (-1);
	}
	free(str);
	return (fd);
}
